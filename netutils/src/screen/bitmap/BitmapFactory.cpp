#include "../../../inc/screen/bitmap/BitmapFactory.hpp"

Bitmap BitmapFactory::createDDBitmap(const HDC hScreenDc, const Size& size) {
    const BITMAPINFOHEADER bi = createBitmapInfoHeader(size);
    const HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDc, size.width, size.height);
    return {hScreenDc, hBitmap, size, bi};
}

Bitmap BitmapFactory::createDIBitmap(const HDC hScreenDc, const Size& size) {
    const BITMAPINFOHEADER bi = createBitmapInfoHeader(size);
    const HBITMAP hBitmap = CreateDIBitmap(hScreenDc, &bi, CBM_INIT, nullptr,
                                           reinterpret_cast<const BITMAPINFO*>(&bi), DIB_RGB_COLORS);
    return {hScreenDc, hBitmap, size, bi};
}

BITMAPINFOHEADER BitmapFactory::createBitmapInfoHeader(const Size& size) {
    BITMAPINFOHEADER bi = {};
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = size.width;
    bi.biHeight = -size.height;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = BYTES_PER_PIXEL * size.width * size.height;
    return bi;
}
