// reactor.hpp
#ifndef REACTOR_HPP
#define REACTOR_HPP
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <poll.h>
#include <thread>
#include <atomic>
#include <iostream>
#include <algorithm>

using namespace std;

template<typename T>
class Reactor {
public:
    using ReactorFunc = std::function<void(int)>;

    Reactor() : running(false) {}

    void* startReactor() {
        running = true;
        reactor_thread = std::thread(&Reactor::run, this);
        return static_cast<void*>(this);
    }

    int addFdToReactor(int fd, ReactorFunc func) {
        if (fd_map.find(fd) != fd_map.end()) {
            return -1;  // FD already exists
        }
        fd_map[fd] = std::move(func);
        return 0;
    }

    int removeFdFromReactor(int fd) {
        return fd_map.erase(fd) ? 0 : -1;
    }

    int stopReactor() {
        running = false;
        if (reactor_thread.joinable()) {
            reactor_thread.join();
        }
        return 0;
    }

    const std::unordered_map<int, ReactorFunc>& getFdMap() const {
        return fd_map;
    }

private:
    void run() {
        while (running) {
            std::vector<pollfd> poll_fds;
            poll_fds.reserve(fd_map.size());
            
            for (const auto& [fd, _] : fd_map) {
                poll_fds.push_back({fd, POLLIN, 0});
            }

            int ret = poll(poll_fds.data(), poll_fds.size(), 100);  // 100ms timeout
            if (ret < 0) {
                perror("poll");
                running = false;
                break;
            } else if (ret == 0) {
                continue; // Timeout, no events
            }

            for (const auto& pfd : poll_fds) {
                if (pfd.revents & POLLIN) {
                    auto it = fd_map.find(pfd.fd);
                    if (it != fd_map.end()) {
                        it->second(pfd.fd);
                    }
                }
            }
        }
    }

    std::unordered_map<int, ReactorFunc> fd_map;
    std::atomic<bool> running;
    std::thread reactor_thread;
};


#endif // REACTOR_HPP