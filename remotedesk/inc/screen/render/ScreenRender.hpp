#pragma once

#include "../DC.hpp"
#include "../../sock/udp/MaskedData.hpp"
#include "../bitmap/Bitmap.hpp"

class ScreenRender {
    DC _dc;
    const SIZE& _appSize;
    Bitmap _diBitmap;
    Bitmap _buffer;
public:
    ScreenRender(const HWND hWnd, const SIZE& appSize, const SIZE& receivedSize);
    void render(const MaskedData& maskedData);
private:
    void ensureBufferMatchesAppSize();
};
