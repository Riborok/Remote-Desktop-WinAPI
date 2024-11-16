#pragma once

#include <vector>

#include "CursorCapture.hpp"
#include "../DC.hpp"
#include "../bitmap/Bitmap.hpp"

class ScreenCapture {
    DC _dc;
    SIZE _originalSize;
    Bitmap _scaledBitmap;
    CursorCapture _cursorCapture;
public:
    explicit ScreenCapture(const SIZE& targetSize);
    std::vector<byte> capture();
};
