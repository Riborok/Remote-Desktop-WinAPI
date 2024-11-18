#pragma once

#include <vector>
#include <windows.h>

class ImageScaler {
public:
    ImageScaler() = delete;

    static std::vector<byte> scaleImage(const std::vector<byte>& src, const SIZE& srcSize, const SIZE& targetSize);
};
