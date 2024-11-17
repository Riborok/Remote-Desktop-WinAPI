#pragma once

#include "../DC.hpp"
#include "../bitmap/Bitmap.hpp"

class ScreenRender {
    DC _dc;
    const SIZE& _appSize;
    Bitmap _bitmap;
public:
    ScreenRender(const HWND hWnd, const SIZE& appSize, const SIZE& receivedSize);
    void render(const std::vector<byte>& screen) const;
};
