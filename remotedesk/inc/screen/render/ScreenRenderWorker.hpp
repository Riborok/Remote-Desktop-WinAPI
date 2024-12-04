#pragma once

#include "ScreenRender.hpp"
#include "../Worker.hpp"

class ScreenRenderWorker final : public Worker {
    ScreenRender _screenRender;
public:
    ScreenRenderWorker(ThreadSafeQueue<std::vector<byte>>& queue, const HWND hWnd,
        const SIZE& appSize, const SIZE& receivedSize, const int fps = 35, const int maxDelayMs = 2000);
    void updateAppSize(const SIZE& appSize);
protected:
    void process() override;
};
