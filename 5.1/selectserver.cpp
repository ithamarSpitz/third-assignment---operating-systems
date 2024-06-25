// selectserver.cpp
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
#include "kosa.hpp"
#include "reactor.hpp"

constexpr const char* PORT = "9034";   // port we're listening on

class Server {
public:
    Server() : reactor(std::make_unique<Reactor<int>>()) {}

    void run() {
        int listener = setup_listener();
        if (listener == -1) {
            std::cerr << "Failed to setup listener\n";
            return;
        }

        reactor->addFdToReactor(listener, [this](int fd) { handle_new_connection(fd); });
        reactor->startReactor();

        std::cout << "Server running. Press 'q' to quit." << std::endl;

        // Non-blocking input handling
        while (true) {
            if (std::cin.rdbuf()->in_avail() > 0) {
                char input;
                std::cin >> input;

                cout << input << endl;
                if (input == 'q') {
                    break;
                }
            }

            // Sleep for a short duration to avoid busy waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        reactor->stopReactor();
        close(listener);
    }


private:
    std::unique_ptr<Reactor<int>> reactor;

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

    void handle_new_connection(int listener) {
        struct sockaddr_storage remoteaddr;
        char remoteIP[INET6_ADDRSTRLEN];
        socklen_t addrlen = sizeof remoteaddr;
        int newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

        if (newfd == -1) {
            perror("accept");
        } else {
            std::cout << "New connection from " 
                      << inet_ntop(remoteaddr.ss_family,
                                   get_in_addr((struct sockaddr*)&remoteaddr),
                                   remoteIP, INET6_ADDRSTRLEN)
                      << " on socket " << newfd << '\n';
        
            reactor->addFdToReactor(newfd, [this](int fd) { handle_client_data(fd); });
        }
    }

    void handle_client_data(int fd) {
        std::cout << "Handling client data on fd: " << fd << std::endl;
        char buf[256];
        int nbytes = recv(fd, buf, sizeof buf, 0);
        if (nbytes <= 0) {
            if (nbytes == 0) {
                std::cout << "Socket " << fd << " hung up\n";
            } else {
                perror("recv");
            }
            close(fd);
            reactor->removeFdFromReactor(fd);
        } else {
            std::string input(buf, nbytes);
            if (input[0] == 'q'){
                reactor->stopReactor();
            }
            std::vector<int> data = parse(input);
            eval(data);
            std::cout << "input: " << input << std::endl;

            // // Send to all other clients
            // for (const auto& [j, _] : reactor->getFdMap()) {
            //     if (j != fd) {
            //         if (send(j, buf, nbytes, 0) == -1) {
            //             perror("send");
            //         }
            //     }
            // }
        }
        reactor->addFdToReactor(fd, [this](int fd) { handle_client_data(fd); });

    }


};

int main() {
    Server server;
    server.run();
    return 0;
}