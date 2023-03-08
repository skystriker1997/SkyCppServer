#include <algorithm>
#include <memory>
#include "Macros.h"
#include "Poller.h"


#ifndef SKYSERVER_EVENTLOOP_H
#define SKYSERVER_EVENTLOOP_H


class EventLoop {
private:
    bool quit_;
    Logger logger_;
    std::unique_ptr<Poller> poller_;

public:
    DISALLOW_COPY_AND_MOVE(EventLoop);
    EventLoop();
    ~EventLoop();

    void Loop();

    int GetEpfd();

    void UpdateChannel(Channel* channel);
    void DeleteChannel(Channel* channel);

    bool CheckQuit() const;
    void Quit();

    unsigned long FdCount();

};

#endif
