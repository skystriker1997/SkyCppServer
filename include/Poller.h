#include <vector>
#include <memory>
#include <sys/epoll.h>
#include "Channel.h"
#include "Macros.h"
#include "Logger.h"
#include <string>
#include <atomic>


#ifndef SKY_POLLER_H
#define SKY_POLLER_H


class Poller {
private:
    int epfd_;
    std::unique_ptr<epoll_event[]> events_;
    Logger logger_;
    std::atomic<long> fd_count_;
    const int max_events_ = 4096;

public:
    DISALLOW_COPY_AND_MOVE(Poller);
    Poller();
    ~Poller();
    int GetEpfd() const;
    void UpdateChannel(Channel* channel);
    void DeleteChannel(Channel* channel);
    std::vector<Channel*> Poll(int timeout);
    bool FdCountLessThan(long n);
    long GetFdCount() const; 
};

#endif
