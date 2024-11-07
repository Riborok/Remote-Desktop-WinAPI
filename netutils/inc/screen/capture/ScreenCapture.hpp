#pragma once

#include <vector>

#include "../DC.hpp"
#include "../Size.hpp"
#include "../bitmap/Bitmap.hpp"

class ScreenCapture {
    DC _dc;
    Bitmap _fullScreenBitmap;
    Bitmap _scaledBitmap;
public:
    explicit ScreenCapture(const Size& targetSize);
    std::vector<BYTE> capture();
};
