﻿#pragma once

#include "ScreenRender.hpp"
#include "../FrameWorker.hpp"

class ScreenRenderWorker final : public FrameWorker {
    ScreenRender _screenRender;
public:
    ScreenRenderWorker(ThreadSafeQueue<std::vector<byte>>& frames, const HWND hWnd,
        const SIZE& appSize, const SIZE& receivedSize, const int fps = 35, const int maxDelayMs = 2000);
    void updateAppSize(const SIZE& appSize);
protected:
    void process() override;
};
