#pragma once

#include <vector>

#include "CursorCapture.hpp"
#include "../DC.hpp"
#include "../bitmap/Bitmap.hpp"

class ScreenCapture {
    DC _dc;
    SIZE _desktopSize;
    SIZE _targetSize;
    Bitmap _bitmap;
    CursorCapture _cursorCapture;
public:
    explicit ScreenCapture(const SIZE& targetSize);
    std::vector<byte> capture();
};
