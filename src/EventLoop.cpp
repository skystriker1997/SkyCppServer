#include "EventLoop.h"



EventLoop::EventLoop() : quit_(false) {
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
    	task_ini = task_ini + "thread id: " + ss.str() + " starts the looping poll!";
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


