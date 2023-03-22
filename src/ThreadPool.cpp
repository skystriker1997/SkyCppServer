#include "ThreadPool.h"



ThreadPool::ThreadPool(unsigned int size) : stop_(false) {
    for(unsigned int i = 0; i < size; ++i) {
        workers_.emplace_back([this](){
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->tasks_mutex_);
                    this->condition_variable_.wait(lock, [this](){return this->stop_ || !this->tasks_.empty();});
                    if(this->stop_ && this->tasks_.empty())     
                    // Note: until there is no task remaining and given the command to stop, do not terminate the thread
                        return;
                    task = this->tasks_.front();
                    this->tasks_.pop();
                }
                task();
            }
	    });
    }
    std::string message;
    message = message + std::to_string(size) + " threads have been created for sevice!";
    logger_.DEBUG(message.c_str());
}



ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        stop_ = true;
    }
    condition_variable_.notify_all();
    while(!tasks_.empty())
        continue;    
	// Note: this is to ensure every task's been undertaken by a thread before the resources of threadpool becoming unpredictable
    std::for_each(std::begin(workers_), std::end(workers_), [](std::thread& one_thread){
                                                                                        if(one_thread.joinable()) 
                                                                                            one_thread.join();
                                                                                       });
}
