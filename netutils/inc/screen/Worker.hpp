#pragma once
#include <atomic>
#include <thread>

class Worker {
    std::chrono::milliseconds _frameDuration;
    std::atomic<bool> _running = false;
    std::thread _workerThread;
protected:
    explicit Worker(const int fps);
public:
    void start();
    void stop();
    virtual ~Worker();

    Worker(Worker&&) = delete;
    Worker& operator=(Worker&&) = delete;
    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;
protected:
    virtual void process() = 0;
private:
    void workLoop();
};
