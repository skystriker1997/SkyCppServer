#include "EventLoop.h"



EventLoop::EventLoop() : quit_(false) {
    poller_ = std::make_unique<Poller>();
}



EventLoop::~EventLoop() = default;



void EventLoop::Loop() {
    while(!this->CheckQuit()) {
        std::vector<Channel*> ready_channels = poller_->Poll(-1);
        std::for_each(std::begin(ready_channels), std::end(ready_channels), [](const auto& channel){channel->HandleEvent();});
    }
}



int EventLoop::GetEpfd() {
    return poller_->GetEpfd();
}



void EventLoop::UpdateChannel(Channel * channel) {
    poller_->UpdateChannel(channel);
}



void EventLoop::DeleteChannel(Channel * channel) {
    poller_->DeleteChannel(channel);
}



unsigned long EventLoop::FdCount() {
    return poller_->FdCount();
}



bool EventLoop::CheckQuit() const {
    return quit_;
}



void EventLoop::Quit() {
    quit_ = true;
}


