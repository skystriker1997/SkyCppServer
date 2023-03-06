#include "Poller.h"


Poller::Poller() : fd_count_(0), epfd_(-1), max_events_(4096) {
    events_ = std::make_unique<epoll_event[]>(max_events_);
    epfd_ = epoll_create1(0);
    if(epfd_ == -1) {
        logger_.ERROR(strerror(errno));
    }
}


Poller::~Poller() {
    if (epfd_ != -1) {
        close(epfd_);
        epfd_ = -1;
    }
}


std::vector<Channel*> Poller::Poll(int timeout){
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


int Poller::GetEpfd() {
    return epfd_;
}


void Poller::UpdateChannel(Channel * channel) {
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


void Poller::DeleteChannel(Channel * channel) {
    int fd = channel->GetFd();
    if(epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr) == -1) {
        logger_.ERROR(strerror(errno));
    } else {
        channel->SetInEpoll(false);
        fd_count_ -= 1;
    }
}


unsigned long Poller::FdCount() {
    return fd_count_;
}


