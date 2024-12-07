#pragma once

#include "ScreenCapture.hpp"
#include "../FrameWorker.hpp"

class ScreenCaptureWorker final : public FrameWorker {
    ScreenCapture _screenCapture;
public:
    ScreenCaptureWorker(ThreadSafeQueue<std::vector<byte>>& frames, const SIZE& targetSize,
        const int fps = 30, const int maxDelayMs = 2000);
protected:
    void process() override;
};
