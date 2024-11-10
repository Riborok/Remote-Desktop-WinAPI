#pragma once

#include "../../../screen/bitmap/Bitmap.hpp"

class BitmapFactory {
    static constexpr DWORD BYTES_PER_PIXEL = 4;
public:
    BitmapFactory() = delete;

    static Bitmap createDDBitmap(const HDC hScreenDc, const SIZE& size);
    static Bitmap createDIBitmap(const HDC hScreenDc, const SIZE& size);
    static DWORD calcBitmapSize(const SIZE& size);
private:
    static BITMAPINFOHEADER createBitmapInfoHeader(const SIZE& size);
};
