
#include "EventLoop.h"


EventLoop::EventLoop() : quit_(false), fd_count_(0) {
    events_ = std::make_unique<epoll_event[]>(max_events_);
    epfd_ = epoll_create1(0);
    if(epfd_ == -1) {
        logger_.ERROR(strerror(errno));
    }
}


EventLoop::~EventLoop() {
    if (epfd_ != -1) {
        close(epfd_);
        epfd_ = -1;
    }
}


void EventLoop::Loop() {
    while(!quit_) {
        std::vector<Channel*> ready_channels = this->Poll(-1);
        std::for_each(std::begin(ready_channels), std::end(ready_channels), [](const auto& channel){channel->HandleEvent();});
    }
}


std::vector<Channel*> EventLoop::Poll(int timeout){
    std::vector<Channel*> active_channels;
    int nfds = epoll_wait(epfd_, events_.get(), max_events_, timeout);
    if(nfds == -1) {
        logger_.ERROR(strerror(errno));
    } else {
        for (int i = 0; i < nfds; ++i) {
            Channel* channel = (Channel*) events_.get()[i].data.ptr;
            channel->SetReadyEvents(events_.get()[i].events);
            active_channels.push_back(channel);
        }
    }
    return active_channels;
}


int EventLoop::GetEpfd() {
    return epfd_;
}


void EventLoop::UpdateChannel(Channel * channel) {
    int fd = channel->GetFd();
    struct epoll_event ev{};
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->GetListenEvents();
    if(!channel->CheckInEpoll()) {
        if(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
            logger_.ERROR(strerror(errno));
        } else {
            fd_count_ += 1;
        }
    } else {
        if(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
            logger_.ERROR(strerror(errno));
        }
    }
}


void EventLoop::DeleteChannel(Channel * channel) {
    int fd = channel->GetFd();
    if(epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr) == -1) {
        logger_.ERROR(strerror(errno));
    } else {
        channel->SetInEpoll(false);
        fd_count_ -= 1;
    }
}


unsigned long EventLoop::GetFdCount() {
    return fd_count_;
}


