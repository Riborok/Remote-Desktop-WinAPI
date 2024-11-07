#pragma once

#include "ScreenCapture.hpp"
#include "../Size.hpp"
#include "../../dt/ThreadSafeQueue.hpp"

class ScreenCaptureWorker {
    ThreadSafeQueue<std::vector<BYTE>>& _queue;
    Size _targetSize;
    int _fps;
    std::atomic<bool> _running = false;
    std::thread _workerThread;
public:
    ScreenCaptureWorker(ThreadSafeQueue<std::vector<BYTE>>& queue, const Size& targetSize, const int fps = 30);
    void start();
    void stop();
    ~ScreenCaptureWorker();

    ScreenCaptureWorker(ScreenCaptureWorker&&) = delete;
    ScreenCaptureWorker& operator=(ScreenCaptureWorker&&) = delete;
    ScreenCaptureWorker(const ScreenCaptureWorker&) = delete;
    ScreenCaptureWorker& operator=(const ScreenCaptureWorker&) = delete;
private:
    void captureLoop() const;
};
