
#include "Channel.h"
#include "EventLoop.h"


Channel::Channel(EventLoop* eloop, int fd) : eloop_(eloop), fd_(fd), listen_events_(0), ready_events_(0), in_epoll_(false) {}

Channel::~Channel() {
    if(fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

int Channel::GetFd() {
    return fd_;
}

uint32_t Channel::GetListenEvents() {
    return listen_events_;
}

uint32_t Channel::GetReadyEvents() {
    return ready_events_;
}

bool Channel::CheckInEpoll() {
    return in_epoll_;
}

void Channel::SetInEpoll(bool in = true) {
    in_epoll_ = in;
}

void Channel::EnableRead() {
    listen_events_ |= EPOLLIN | EPOLLPRI;
    eloop_->UpdateChannel(this);
}


void Channel::EnableWrite() {
    listen_events_ |= EPOLLOUT;
}


void Channel::SetReadyEvents(uint32_t ev) {
    ready_events_ |= ev;
}


void Channel::HandleEvent() {
    if (ready_events_ & (EPOLLIN | EPOLLPRI)) {  //EPOLLIN means the associated file is available for read(2) operations.
        read_callback_();                        //EPOLLPRI means urgent data are accessible, or say out-of-band data arrived
    }
    if (ready_events_ & EPOLLOUT) {   //EPOLLOUT means the associated file is available for write(2) operations
        write_callback_();
    }
}




