#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include "reactor1.hpp"

typedef struct fd_callback {
    int fd;
    reactorFunc func;
} fd_callback;

struct reactor_t {
    fd_set master;
    fd_set read_fds;
    int fdmax;
    fd_callback callbacks[FD_SETSIZE];
};

reactor* startReactor() {
    reactor* r = (reactor*)malloc(sizeof(reactor));
    FD_ZERO(&r->master);
    FD_ZERO(&r->read_fds);
    r->fdmax = -1;
    return r;
}

int addFdToReactor(reactor* r, int fd, reactorFunc func) {
    if (fd < 0 || fd >= FD_SETSIZE) {
        return -1;
    }
    FD_SET(fd, &r->master);
    if (fd > r->fdmax) {
        r->fdmax = fd;
    }
    r->callbacks[fd].fd = fd;
    r->callbacks[fd].func = func;
    return 0;
}

int removeFdFromReactor(reactor* r, int fd) {
    if (fd < 0 || fd >= FD_SETSIZE) {
        return -1;
    }
    FD_CLR(fd, &r->master);
    if (fd == r->fdmax) {
        for (int i = r->fdmax - 1; i >= 0; i--) {
            if (FD_ISSET(i, &r->master)) {
                r->fdmax = i;
                break;
            }
        }
    }
    r->callbacks[fd].fd = -1;
    r->callbacks[fd].func = NULL;
    return 0;
}

int stopReactor(reactor* r) {
    free(r);
    return 0;
}

void runReactor(reactor* r) {
    while (1) {
        r->read_fds = r->master;
        if (select(r->fdmax + 1, &r->read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        for (int i = 0; i <= r->fdmax; i++) {
            if (FD_ISSET(i, &r->read_fds)) {
                if (r->callbacks[i].func != NULL) {
                    r->callbacks[i].func(i);
                }
            }
        }
    }
}
