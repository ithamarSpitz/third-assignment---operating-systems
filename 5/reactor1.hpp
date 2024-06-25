#ifndef REACTOR1_HPP
#define REACTOR1_HPP

typedef void (*reactorFunc)(int fd);

typedef struct reactor_t reactor;

// starts a new reactor and returns pointer to it
reactor* startReactor();

// adds fd to Reactor (for reading); returns 0 on success
int addFdToReactor(reactor* r, int fd, reactorFunc func);

// removes fd from reactor
int removeFdFromReactor(reactor* r, int fd);

// stops reactor
int stopReactor(reactor* r);

// runs the reactor
void runReactor(reactor* r);

#endif
