// reactor.c
#include "reactor.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h>

Reactor* startReactor() {
    Reactor* reactor = (Reactor*)malloc(sizeof(Reactor));
    if (reactor == NULL) {
        return NULL;
    }
    FD_ZERO(&reactor->master);
    FD_ZERO(&reactor->read_fds);
    reactor->fdmax = -1;
    reactor->funcs = (reactorFunc*)calloc(FD_SETSIZE, sizeof(reactorFunc));
    if (reactor->funcs == NULL) {
        free(reactor);
        return NULL;
    }
    return reactor;
}

int addFdToReactor(Reactor* reactor, int fd, reactorFunc func) {
    if (fd >= FD_SETSIZE) {
        return -1; // fd out of range
    }
    FD_SET(fd, &reactor->master);
    if (fd > reactor->fdmax) {
        reactor->fdmax = fd;
    }
    reactor->funcs[fd] = func;
    return 0;
}

int removeFdFromReactor(Reactor* reactor, int fd) {
    if (fd >= FD_SETSIZE) {
        return -1; // fd out of range
    }
    FD_CLR(fd, &reactor->master);
    if (fd == reactor->fdmax) {
        while (reactor->fdmax >= 0 && !FD_ISSET(reactor->fdmax, &reactor->master)) {
            reactor->fdmax--;
        }
    }
    reactor->funcs[fd] = NULL;
    return 0;
}

int stopReactor(Reactor* reactor) {
    if (reactor == NULL) {
        return -1;
    }
    free(reactor->funcs);
    free(reactor);
    return 0;
}

void runReactor(Reactor* reactor) {
    while (1) {
        reactor->read_fds = reactor->master;
        if (select(reactor->fdmax + 1, &reactor->read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }

        for (int i = 0; i <= reactor->fdmax; i++) {
            if (FD_ISSET(i, &reactor->read_fds)) {
                if (reactor->funcs[i] != NULL) {
                    reactor->funcs[i](i);
                }
            }
        }
    }
}
