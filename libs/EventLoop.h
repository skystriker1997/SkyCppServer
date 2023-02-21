
#include <vector>
#include <algorithm>
#include <cstdio>
#include <memory>
#include <unistd.h>
#include <cstring>
#include <sys/epoll.h>
#include <array>
#include "Channel.h"
#include "Macros.h"
#include "Logger.h"


#ifndef TINYWEBSERVER_EVENTLOOP_H
#define TINYWEBSERVER_EVENTLOOP_H


class EventLoop
{
private:
    int epfd_ = -1;
    bool quit_;
    std::unique_ptr<epoll_event[]> events_;
    const int max_events_ = 1024;
    Logger logger_;
    unsigned long fd_count_;
public:
    DISALLOW_COPY_AND_MOVE(EventLoop);
    EventLoop();
    ~EventLoop();
    void Loop();
    int GetEpfd();
    void UpdateChannel(Channel* channel);
    void DeleteChannel(Channel* channel);
    std::vector<Channel*> Poll(int timeout);
    unsigned long GetFdCount();
};

#endif //TINYWEBSERVER_EVENTLOOP_H
