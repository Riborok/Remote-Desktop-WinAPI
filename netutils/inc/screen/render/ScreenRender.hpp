#pragma once

#include "../DC.hpp"
#include "../../sock/udp/MaskedData.hpp"
#include "../bitmap/Bitmap.hpp"

class ScreenRender {
    DC _dc;
    Size& _appSize;
    Bitmap _diBitmap;
public:
    ScreenRender(const HWND hWnd, Size& appSize, const Size& receivedSize);
    void render(const MaskedData& maskedData);
};
