#pragma once

#include "Bitmap.hpp"

class BitmapFactory {
    static constexpr DWORD BYTES_PER_PIXEL = 4;
public:
    BitmapFactory() = delete;

    static Bitmap createDDBitmap(const HDC hScreenDc, const Size& size);
    static Bitmap createDIBitmap(const HDC hScreenDc, const Size& size);
private:
    static BITMAPINFOHEADER createBitmapInfoHeader(const Size& size);
};
