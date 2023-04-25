#include "Poller.h"
#include "Global.h"

Poller::Poller() : fd_count_(0), epfd_(-1), max_events_(4096), logger_(Logger::log_level::debug, Logger::log_target::file_and_terminal, log_path) {
    events_ = std::make_unique<epoll_event[]>(max_events_);
    epfd_ = epoll_create1(0);
    if(epfd_ == -1) {
        std::string message = "failed to create epoll, error info: ";
        message += strerror(errno);
        logger_.ERROR(message.c_str());
    }
}



Poller::~Poller() {
    if(epfd_ != -1) {
        close(epfd_);
        epfd_ = -1;
    }
}



std::vector<Channel*> Poller::Poll(int timeout){
    std::vector<Channel*> active_channels;
    std::string message = "epfd.";
    int nfds = epoll_wait(epfd_, events_.get(), max_events_, timeout);
    if(nfds == -1) {
        message = message + std::to_string(epfd_) + " encountered error when waiting events, error info: " + strerror(errno);
        logger_.ERROR(message.c_str());
    } else {
            for (int i = 0; i < nfds; ++i) {
                auto* channel = (Channel*) events_.get()[i].data.ptr;
                channel->SetReadyEvents(events_.get()[i].events);
                active_channels.push_back(channel);
            }
    }
    message.clear();
    message = message + "epfd." + std::to_string(epfd_) + " has " + std::to_string(active_channels.size()) + " active channels!";
    logger_.DEBUG(message.c_str());
    return active_channels;
}



int Poller::GetEpfd() const {
    return epfd_;
}



void Poller::UpdateChannel(Channel * channel) {
    int fd = channel->GetFd();
    struct epoll_event ev{};
    ev.data.ptr = channel;
    ev.events = channel->GetListenEvents();
    std::string message = "epfd.";
    if(!channel->CheckInEpoll()) {
        if(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
            message = message + std::to_string(epfd_) + " failed to pick up sockfd." + std::to_string(fd) + ", error info: " + strerror(errno);
            logger_.ERROR(message.c_str());
        } else {
            channel->SetInEpoll(true);
            ++fd_count_;
            message = message + std::to_string(epfd_) + " picked up sockfd." + std::to_string(fd) + ", which listens " + \
					  std::to_string(channel->GetListenEvents());
            logger_.DEBUG(message.c_str());
        }
    } else {
            if(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
                message = message + std::to_string(epfd_) + " failed to modify sockfd." + std::to_string(fd) + \
						  ", error info: " + strerror(errno);
                logger_.ERROR(message.c_str());
            } else {
                message = message + std::to_string(epfd_) + " modified sockfd." + std::to_string(fd) + ", which listens " + \
						  std::to_string(channel->GetListenEvents());
            }
    }
}


void Poller::DeleteChannel(Channel * channel) {
    int fd = channel->GetFd();
    std::string message = "epfd.";
    if(epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr) == -1) {
        message = message + std::to_string(epfd_) + " failed to delete sockfd." + std::to_string(fd) + ", error info: " + strerror(errno);
        logger_.ERROR(message.c_str());
    } else {
        channel->SetInEpoll(false);
        --fd_count_;
        message = message + std::to_string(epfd_) + " deleted sockfd." + std::to_string(fd);
        logger_.DEBUG(message.c_str());
    }
}



bool Poller::FdCountLessThan(long n) {
    return fd_count_ < n;
}


long Poller::GetFdCount() const {
    return fd_count_.load();
}
