// reactor.h
#ifndef REACTOR_H
#define REACTOR_H


#include <sys/select.h> // For select
#include <sys/time.h>   // For timeval
#include <sys/types.h>  // For fd_set
#include <unistd.h>     // For select

typedef void (*reactorFunc)(int fd);

typedef struct Reactor {
    fd_set master;        // Master file descriptor list
    fd_set read_fds;      // Temp file descriptor list for select()
    int fdmax;            // Maximum file descriptor number
    reactorFunc* funcs;   // Array of function pointers
} Reactor;

// Starts new reactor and returns pointer to it
Reactor* startReactor();

// Adds fd to Reactor (for reading); returns 0 on success.
int addFdToReactor(Reactor* reactor, int fd, reactorFunc func);

// Removes fd from reactor
int removeFdFromReactor(Reactor* reactor, int fd);

// Stops reactor
int stopReactor(Reactor* reactor);

#endif // REACTOR_H
