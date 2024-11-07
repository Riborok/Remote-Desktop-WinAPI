#pragma once

#include "ScreenCapture.hpp"
#include "../Worker.hpp"

#include "../../dt/ThreadSafeQueue.hpp"

class ScreenCaptureWorker final : public Worker {
    ThreadSafeQueue<std::vector<BYTE>>& _queue;
    ScreenCapture _screenCapture;
public:
    ScreenCaptureWorker(ThreadSafeQueue<std::vector<BYTE>>& queue, const SIZE& targetSize, const int fps = 30);
protected:
    void process() override;
};
