#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t threadCount);
    ~ThreadPool();

    void Enqueue(const std::function<void()>& job);

private:
    void Worker();

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;

    std::mutex queueMutex;
    std::condition_variable cv;
    std::atomic<bool> stop;
};

#endif