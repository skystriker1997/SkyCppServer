#include "ThreadPool.h"



ThreadPool::ThreadPool(unsigned int size) : stop_(false) {
    for(unsigned int i = 0; i < size; ++i) {
        workers_.emplace_back([this](){   // Note: emplace_back forward the argument along and construct it directly in place with no copies or moves
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->tasks_mutex_);
                    this->condition_variable_.wait(lock, [this](){return this->stop_ || !this->tasks_.empty();});
                    if(this->stop_ && this->tasks_.empty())     // Note: Until there is no task left given the command of stop, do not terminate the thread
                        return;
                    task = this->tasks_.front();
                    this->tasks_.pop();
                }
                task();
            }
        });
    }
}



ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        stop_ = true;
    }
    condition_variable_.notify_all();
    while(!tasks_.empty())
        continue;    // Note: To ensure every task is undertaken by a thread before the resources of ThreadPool becomes uncontrollable
    std::for_each(std::begin(workers_), std::end(workers_),[](std::thread& one_thread){if(one_thread.joinable()) one_thread.join();});
}