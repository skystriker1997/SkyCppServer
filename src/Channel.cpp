#include "Channel.h"
#include "EventLoop.h"
#include "Global.h"


Channel::Channel(EventLoop* eloop, int fd) : eloop_(eloop), fd_(fd), listen_events_(0), ready_events_(0), in_epoll_(false), logger_(Logger::log_level::debug, Logger::log_target::file_and_terminal, log_path) {}



Channel::~Channel() {
    if(CheckInEpoll()) {
        eloop_->DeleteChannel(this);
    }
}



int Channel::GetFd() const {
    return fd_;
}



uint32_t Channel::GetListenEvents() const {
    return listen_events_;
}



uint32_t Channel::GetReadyEvents() const {
    return ready_events_;
}



bool Channel::CheckInEpoll() const {
    return in_epoll_;
}



void Channel::SetInEpoll(bool in) {
    in_epoll_ = in;
}



void Channel::EnableRead() {
    listen_events_ = EPOLLIN | EPOLLPRI;
    eloop_->UpdateChannel(this);
}



void Channel::EnableWrite() {
    listen_events_ |= EPOLLOUT;
    eloop_->UpdateChannel(this);
}



void Channel::SetReadyEvents(uint32_t ev) {
    ready_events_ |= ev;
}



void Channel::HandleEvent() {
    std::string message = "the channel for sockfd.";
    message = message + std::to_string(fd_) + " listens " + std::to_string(GetListenEvents()) + \
			  " and is ready with " + std::to_string(GetReadyEvents());
	logger_.DEBUG(message.c_str());
    if(GetReadyEvents() & (EPOLLIN | EPOLLPRI)) {
        read_callback_();
    }
    if(GetReadyEvents() & EPOLLOUT) {
        write_callback_();
    }
}




