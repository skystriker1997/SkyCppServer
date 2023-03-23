#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Macros.h"
#include <algorithm>
#include <memory>
#include <cstdio>
#include <string>
#include "Logger.h"


#ifndef SKY_THREADPOOL_H
#define SKY_THREADPOOL_H


class ThreadPool {
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasks_mutex_;
    std::condition_variable condition_variable_;
    bool stop_;
    Logger logger_;


public:
    DISALLOW_COPY_AND_MOVE(ThreadPool);

    explicit ThreadPool(unsigned int size);

    ~ThreadPool();

    template<typename F>
    bool AddTask(F&& func) {
        if(!stop_) {
            {
                std::unique_lock<std::mutex> lock(tasks_mutex_);
                tasks_.emplace(func);
            }
            condition_variable_.notify_one();
            return true;
        } else {
            return false;
        }
    };
};


#endif
