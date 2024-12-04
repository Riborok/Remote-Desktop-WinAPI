#pragma once

#include <atomic>
#include <thread>

#include "QueueSizeMonitor.hpp"
#include "../types/byte.hpp"

class Worker {
    ThreadSafeQueue<std::vector<byte>>& _queue;
    QueueSizeMonitor<std::vector<byte>> _queueSizeMonitor;
    
    std::atomic<long long> _frameDuration;
    std::atomic<bool> _running = false;
    std::thread _workerThread;
protected:
    explicit Worker(ThreadSafeQueue<std::vector<byte>>& queue, const int fps, const int maxDelayMs);
    ThreadSafeQueue<std::vector<byte>>& getQueue() const;
public:
    void start();
    void stop();
    void updateFPSAndMaxDelay(const int fps, const int maxDelayMs);
    virtual ~Worker();

    Worker(Worker&&) = delete;
    Worker& operator=(Worker&&) = delete;
    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;
protected:
    virtual void process() = 0;
private:
    void setFrameDuration(const int fps);
    void workLoop();
};
