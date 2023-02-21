
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Macros.h"
#include <algorithm>
#include <memory>


#ifndef TINYWEBSERVER_THREADPOOL_H
#define TINYWEBSERVER_THREADPOOL_H


class ThreadPool
{
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasks_mutex_;
    std::condition_variable condition_variable_;
    bool stop_;

public:
    DISALLOW_COPY_AND_MOVE(ThreadPool);

    ThreadPool(unsigned int size = std::thread::hardware_concurrency());

    ~ThreadPool();

    bool AddTask(const std::function<void()>& func);
};


#endif //TINYWEBSERVER_THREADPOOL_H
