﻿#pragma once

#include "ScreenCapture.hpp"
#include "../Worker.hpp"

class ScreenCaptureWorker final : public Worker {
    ScreenCapture _screenCapture;
public:
    ScreenCaptureWorker(ThreadSafeQueue<std::vector<byte>>& queue, const SIZE& targetSize,
        const int fps = 30, const int maxDelayMs = 2000);
protected:
    void process() override;
};
