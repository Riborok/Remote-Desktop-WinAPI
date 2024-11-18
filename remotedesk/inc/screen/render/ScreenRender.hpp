#pragma once

#include <mutex>

#include "../DC.hpp"
#include "../bitmap/Bitmap.hpp"

class ScreenRender {
    DC _dc;
    SIZE _appSize;
    SIZE _originalSize;
    Bitmap _bitmap;
    std::mutex _appSizeMutex;
public:
    ScreenRender(const HWND hWnd, const SIZE& appSize, const SIZE& originalSize);
    void render(const std::vector<byte>& screen);
    void updateAppSize(const SIZE& appSize);
};
