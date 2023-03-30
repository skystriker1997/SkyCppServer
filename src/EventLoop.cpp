#include "EventLoop.h"
#include "Global.h"


EventLoop::EventLoop() : quit_(false), logger_(Logger::log_level::debug, Logger::log_target::file_and_terminal, log_path) {
    poller_ = std::make_unique<Poller>();
}



EventLoop::~EventLoop() {
    this->Quit();
};



void EventLoop::Loop() {
    std::thread::id thread_id = std::this_thread::get_id();
    std::ostringstream ss;
    ss << thread_id;
    std::string task_ini;
    task_ini = task_ini + "epfd." + std::to_string(this->GetEpfd()) + " starts the looping poll on thread_id." + ss.str();
    logger_.DEBUG(task_ini.c_str());
    while(!this->CheckQuit()) {
        std::vector<Channel*> ready_channels = poller_->Poll(-1);
        std::for_each(std::begin(ready_channels), std::end(ready_channels), [](const auto& channel){channel->HandleEvent();});
    }
    std::string task_termi;
    task_termi = task_termi + "thread id: " + ss.str() + " ends the looping poll!";
    logger_.DEBUG(task_termi.c_str());
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


