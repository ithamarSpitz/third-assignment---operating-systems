#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "kosa.hpp"
#include <string>
#include <vector>
#include <algorithm>


#define PORT "9034"

// Shared graph data structure
std::vector<std::vector<int>> graph;
pthread_mutex_t graph_mutex = PTHREAD_MUTEX_INITIALIZER;

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void handle_client(int client_fd)
{
    char buf[256];
    int nbytes;

    while(1) {
        if ((nbytes = recv(client_fd, buf, sizeof buf, 0)) <= 0) {
            if (nbytes == 0) {
                printf("Client on socket %d disconnected\n", client_fd);
            } else {
                perror("recv");
            }
            close(client_fd);
            return;
        }

        buf[nbytes] = '\0';
        std::string input(buf);
        std::vector<int> data = parse(input);

        pthread_mutex_lock(&graph_mutex);

        if (input.find("new_graph") != std::string::npos) {
            graph.clear();
            graph.resize(data[1]);
        } else if (input.find("add_edge") != std::string::npos) {
            if (data[1] < graph.size() && data[2] < graph.size()) {
                graph[data[1]].push_back(data[2]);
            }
        } else if (input.find("remove_edge") != std::string::npos) {
            if (data[1] < graph.size()) {
                auto it = std::find(graph[data[1]].begin(), graph[data[1]].end(), data[2]);
                if (it != graph[data[1]].end()) {
                    graph[data[1]].erase(it);
                }
            }
        } else if (input.find("kosaraju") != std::string::npos) {
            std::vector<std::vector<int>> result = kosaraju();
            std::string response = "Kosaraju result: ";
            for (const auto& scc : result) {
                response += "[";
                for (int v : scc) {
                    response += std::to_string(v) + ",";
                }
                response += "] ";
            }
            send(client_fd, response.c_str(), response.length(), 0);
        }

        pthread_mutex_unlock(&graph_mutex);

        send(client_fd, "Command processed", 18, 0);
    }
}

void *client_thread(void *arg)
{
    int client_fd = *((int*)arg);
    free(arg);
    handle_client(client_fd);
    return NULL;
}

int main(void)
{
    int listener;
    int newfd;
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen;
    char remoteIP[INET6_ADDRSTRLEN];
    int yes=1;
    int rv;
    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) continue;
        
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai);

    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    printf("Server: waiting for connections...\n");

    while(1) {
        addrlen = sizeof remoteaddr;
        newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

        if (newfd == -1) {
            perror("accept");
            continue;
        }

        printf("Server: new connection from %s on socket %d\n",
            inet_ntop(remoteaddr.ss_family,
                get_in_addr((struct sockaddr*)&remoteaddr),
                remoteIP, INET6_ADDRSTRLEN),
            newfd);

        int *client_fd = (int*)malloc(sizeof(int));
        *client_fd = newfd;
        pthread_t thread;
        pthread_create(&thread, NULL, client_thread, client_fd);
        pthread_detach(thread);
    }

    return 0;
}