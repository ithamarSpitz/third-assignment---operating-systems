#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "reactor1.hpp"
#include "kosa.hpp"

#define PORT "9034"   // port we're listening on

// get sockaddr, IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void handle_client(int fd) {
    char buf[256];
    int nbytes;
    
    if ((nbytes = recv(fd, buf, sizeof buf, 0)) <= 0) {
        // got error or connection closed by client
        if (nbytes == 0) {
            // connection closed
            printf("server: socket %d hung up\n", fd);
        } else {
            perror("recv");
        }
        close(fd);
    } else {
        // we got some data from a client
        buf[nbytes] = '\0';
        printf("Received: %s\n", buf);

        //std::string input(buf);
        //std::vector<int> data = parse(input); // from kosa.hpp
        //eval(data); // from kosa.hpp
    }
}

void handle_new_connection(int listener) {
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen = sizeof remoteaddr;
    char remoteIP[INET6_ADDRSTRLEN];
    
    int newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);
    if (newfd == -1) {
        perror("accept");
    } else {
        reactor* r = startReactor();
        addFdToReactor(r, newfd, handle_client);
        printf("server: new connection from %s on socket %d\n",
               inet_ntop(remoteaddr.ss_family,
                         get_in_addr((struct sockaddr*)&remoteaddr),
                         remoteIP, INET6_ADDRSTRLEN),
               newfd);
    }
}

int main(void) {
    reactor* r = startReactor();
    int listener;
    int yes = 1;
    struct addrinfo hints, *ai, *p;
    int rv;  // Declaring rv here

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "server: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for(p = ai; p != NULL; p = p->ai_next) {
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
        fprintf(stderr, "server: failed to bind\n");
        exit(2);
    }
    freeaddrinfo(ai);
    
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }
    
    addFdToReactor(r, listener, handle_new_connection);
    runReactor(r);
    stopReactor(r);
    
    return 0;
}
