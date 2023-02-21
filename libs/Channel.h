
#include <sys/epoll.h>
#include <functional>
#include "Macros.h"
#include <unistd.h>
#include <memory>

#ifndef TINYWEBSERVER_CHANNEL_H
#define TINYWEBSERVER_CHANNEL_H

class EventLoop;

class Channel {
private:
    EventLoop* eloop_;
    int fd_;
    uint32_t listen_events_;
    uint32_t ready_events_;
    bool in_epoll_;
    std::function<void()> read_callback_;
    std::function<void()> write_callback_;



public:
    DISALLOW_COPY_AND_MOVE(Channel);
    Channel(EventLoop* eloop, int fd);
    ~Channel();

    int GetFd();
    uint32_t GetListenEvents();
    uint32_t GetReadyEvents();
    bool CheckInEpoll();
    void SetInEpoll(bool in);

    void EnableRead();
    void EnableWrite();

    void SetReadyEvents(uint32_t ev);
    void SetReadCallback(std::function<void()>&& callback);
    void SetWriteCallback(std::function<void()>&& callback);

    void HandleEvent();

};



#endif //TINYWEBSERVER_CHANNEL_H
