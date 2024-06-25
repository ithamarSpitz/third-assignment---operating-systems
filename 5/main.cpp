// main.c
#include "reactor.hpp"
#include "kosa.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "9034"   // port we're listening on

Reactor* reactor;

void handleClient(int fd) {
    char buf[256];
    int nbytes;
    if ((nbytes = recv(fd, buf, sizeof buf, 0)) <= 0) {
        if (nbytes == 0) {
            printf("selectserver: socket %d hung up\n", fd);
        } else {
            perror("recv");
        }
        close(fd);
        removeFdFromReactor(reactor, fd);
    } else {
        printf("Received: %s\n", buf);
        if (send(fd, buf, nbytes, 0) == -1) {
            perror("send");
        }
    }
}

int main(void) {
    reactor = startReactor();
    if (reactor == NULL) {
        perror("Failed to start reactor");
        exit(1);
    }

    int listener;
    int yes = 1;
    struct addrinfo hints, *ai, *p;
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen;
    int newfd;

    FD_ZERO(&reactor->master);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    int rv;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

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

    if (addFdToReactor(reactor, listener, handleClient) != 0) {
        perror("Failed to add listener to reactor");
        exit(1);
    }

    while (1) {
        reactor->read_fds = reactor->master;
        if (select(reactor->fdmax + 1, &reactor->read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        for (int i = 0; i <= reactor->fdmax; i++) {
            if (FD_ISSET(i, &reactor->read_fds)) {
                if (i == listener) {
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        printf("selectserver: new connection on socket %d\n", newfd);
                        if (addFdToReactor(reactor, newfd, handleClient) != 0) {
                            perror("Failed to add newfd to reactor");
                            exit(1);
                        }
                    }
                } else {
                    handleClient(i);
                }
            }
        }
    }

    stopReactor(reactor);
    return 0;
}
