#pragma once

#include "ScreenRender.hpp"
#include "../Worker.hpp"

#include "../../dt/ThreadSafeQueue.hpp"

class ScreenRenderWorker final : public Worker {
    ThreadSafeQueue<std::vector<byte>>& _queue;
    ScreenRender _screenRender;
public:
    ScreenRenderWorker(ThreadSafeQueue<std::vector<byte>>& queue,
        const HWND hWnd, const SIZE& appSize, const SIZE& receivedSize, const int fps = 30);
protected:
    void process() override;
};
