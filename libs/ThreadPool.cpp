
#include "ThreadPool.h"



ThreadPool::ThreadPool(unsigned int size) : stop_(false) {
    for(unsigned int i = 0; i < size; ++i) {
        workers_.emplace_back(std::thread([this](){   //based on the code as written in standard libraries, emplace_back forward the argument along and construct it directly in place with no copies or moves
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->tasks_mutex_);
                    this->condition_variable_.wait(lock, [this](){return this->stop_ || !this->tasks_.empty();});  // if any task is available, or should stop the thread, just go ahead
                    if(this->stop_ && this->tasks_.empty())     // when there is no task left after the command of stop, just terminate the thread
                        return;
                    task = this->tasks_.front();
                    this->tasks_.pop();
                }
                task();    // in this case, task is an infinite loop of epoll until quit is specified
            }
        }));
    }
}

//overall, in this case, each thread matches a EventLoop, when EventLoop asks quit and ThreadPool asks stop, thread


ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        stop_ = true;
    }
    condition_variable_.notify_all();
    while(!tasks_.empty())
        continue;    // ensure every task is undertaken by a thread before the resources of ThreadPool becomes uncontrollable
    std::for_each(std::begin(workers_), std::end(workers_),[](std::thread& one_thread){if(one_thread.joinable()) one_thread.join();});
}