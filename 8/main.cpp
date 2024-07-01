// main.cpp
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include "kosa.hpp"
#include "reactor.hpp"

Graph g;
std::mutex graph_mutex;
constexpr const char* PORT = "9034";

class Server {
public:
    Server() : proactor(std::make_unique<Proactor<int>>()) {}

    void run() {
        int listener = setup_listener();
        if (listener == -1) {
            std::cerr << "Failed to setup listener\n";
            return;
        }

        pthread_t proactor_tid = proactor->startProactor(listener, [this](int fd) { handle_client(fd); });

        std::cout << "Server running. Press 'q' to quit." << std::endl;

        while (true) {
            if (std::cin.rdbuf()->in_avail() > 0) {
                char input;
                std::cin >> input;
                if (input == 'q') {
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        proactor->stopProactor(proactor_tid);
        close(listener);
    }

private:
    std::unique_ptr<Proactor<int>> proactor;

    static void* get_in_addr(struct sockaddr *sa) {
        if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
        }
        return &(((struct sockaddr_in6*)sa)->sin6_addr);
    }

    int setup_listener() {
        struct addrinfo hints{}, *ai, *p;
        int listener;
        int yes = 1;
        int rv;

        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        if ((rv = getaddrinfo(nullptr, PORT, &hints, &ai)) != 0) {
            std::cerr << "getaddrinfo: " << gai_strerror(rv) << '\n';
            return -1;
        }
    
        for(p = ai; p != nullptr; p = p->ai_next) {
            listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (listener < 0) continue;
        
            setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

            if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
                close(listener);
                continue;
            }

            break;
        }

        freeaddrinfo(ai);

        if (p == nullptr) {
            std::cerr << "Failed to bind\n";
            return -1;
        }

        if (listen(listener, 10) == -1) {
            perror("listen");
            return -1;
        }

        return listener;
    }

void handle_client(int fd) {
    std::string buffer;
    char buf[256];
    while (true) {
        int nbytes = recv(fd, buf, sizeof buf, 0);
        if (nbytes <= 0) {
            if (nbytes == 0) {
                std::cout << "Socket " << fd << " hung up\n";
            } else {
                perror("recv");
            }
            close(fd);
            return;
        }

        buffer.append(buf, nbytes);

        size_t pos;
        while ((pos = buffer.find('\n')) != std::string::npos) {
            std::string command = buffer.substr(0, pos);
            buffer.erase(0, pos + 1);

            std::cout << "Received command: " << command << std::endl;

            std::vector<string> data = g.parse(command);

            std::lock_guard<std::mutex> lock(graph_mutex);
            bool result = g.eval(data);

            // Send response to client
            std::string response = result ? "Command processed successfully\n" : "Command processing failed\n";
            send(fd, response.c_str(), response.length(), 0);
            
            std::cout << "Sent response: " << response;
        }
    }
}
};

int main() {
    Server server;
    server.run();
    return 0;
}