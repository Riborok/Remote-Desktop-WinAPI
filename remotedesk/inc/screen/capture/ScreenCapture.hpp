#pragma once

#include <vector>

#include "../DC.hpp"
#include "../bitmap/Bitmap.hpp"

class ScreenCapture {
    DC _dc;
    Bitmap _fullScreenBitmap;
    Bitmap _scaledBitmap;
public:
    explicit ScreenCapture(const SIZE& targetSize);
    std::vector<BYTE> capture();
};
