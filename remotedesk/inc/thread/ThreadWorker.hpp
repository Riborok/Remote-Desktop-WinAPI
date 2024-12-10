#pragma once

#include <atomic>
#include <thread>

class ThreadWorker {
    std::thread _eventThread;
protected:
    std::atomic<bool> _running = false;
    ThreadWorker() = default;
public:
    void start();
    void stopRunning();
    void waitForThread();
    
    virtual ~ThreadWorker();
    ThreadWorker(ThreadWorker&&) = delete;
    ThreadWorker& operator=(ThreadWorker&&) = delete;
    ThreadWorker(const ThreadWorker&) = delete;
    ThreadWorker& operator=(const ThreadWorker&) = delete;
protected:
    virtual void eventLoop() = 0;
};
