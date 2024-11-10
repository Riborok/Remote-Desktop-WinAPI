#include "../../../../inc/utils/factory/bitmap/BitmapFactory.hpp"

Bitmap BitmapFactory::createDDBitmap(const HDC hScreenDc, const SIZE& size) {
    const BITMAPINFOHEADER bi = createBitmapInfoHeader(size);
    const HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDc, size.cx, size.cy);
    return {hScreenDc, hBitmap, size, bi};
}

Bitmap BitmapFactory::createDIBitmap(const HDC hScreenDc, const SIZE& size) {
    const BITMAPINFOHEADER bi = createBitmapInfoHeader(size);
    const HBITMAP hBitmap = CreateDIBitmap(hScreenDc, &bi, CBM_INIT, nullptr,
        reinterpret_cast<const BITMAPINFO*>(&bi), DIB_RGB_COLORS);
    return {hScreenDc, hBitmap, size, bi};
}

BITMAPINFOHEADER BitmapFactory::createBitmapInfoHeader(const SIZE& size) {
    BITMAPINFOHEADER bi = {};
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = size.cx;
    bi.biHeight = -size.cy;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = calcBitmapSize(size);
    return bi;
}

DWORD BitmapFactory::calcBitmapSize(const SIZE& size) {
    return BYTES_PER_PIXEL * size.cx * size.cy;
}