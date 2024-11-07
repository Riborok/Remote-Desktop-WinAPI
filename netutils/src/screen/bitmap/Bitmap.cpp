#include "../../../inc/screen/bitmap/Bitmap.hpp"

#include "../../../inc/utils/screen/ScreenUtils.hpp"

Bitmap::Bitmap(const HDC hScreenDc, const HBITMAP hMemoryBitmap, const Size& size, const BITMAPINFOHEADER& bi):
    _hMemoryDc(CreateCompatibleDC(hScreenDc)), _hMemoryBitmap(hMemoryBitmap),
    _oldBitmap(SelectObject(_hMemoryDc, _hMemoryBitmap)), _size(size), _bi(bi) { }

void Bitmap::enableHighQualityStretching() const {
    ScreenUtils::enableHighQualityStretching(_hMemoryDc);
}

void Bitmap::copyFrom(const HDC src) const {
    BitBlt(_hMemoryDc, 0, 0, _size.width, _size.height, src, 0, 0, SRCCOPY);
}

void Bitmap::copyTo(const HDC dest) const {
    BitBlt(dest, 0, 0, _size.width, _size.height, _hMemoryDc, 0, 0, SRCCOPY);
}

void Bitmap::stretchFrom(const Bitmap& src, const DWORD rop) const {
    stretchFrom(src._hMemoryDc, src._size, rop);
}

void Bitmap::stretchFrom(const HDC src, const Size& srcSize, const DWORD rop) const {
    StretchBlt(_hMemoryDc, 0, 0, 
        _size.width, _size.height,
        src, 0, 0,
        srcSize.width, srcSize.height, rop
    );
}

void Bitmap::stretchTo(const Bitmap& dest, const DWORD rop) const {
    stretchTo(dest._hMemoryDc, dest._size, rop);
}

void Bitmap::stretchTo(const HDC dest, const Size& destSize, const DWORD rop) const {
    StretchBlt(dest, 0, 0, 
        destSize.width, destSize.height,
        _hMemoryDc, 0, 0, 
        _size.width, _size.height, rop
    );
}

std::vector<BYTE> Bitmap::getDIBits() {
    std::vector<BYTE> buffer(_bi.biSizeImage);
    GetDIBits(_hMemoryDc, _hMemoryBitmap, 0, _size.height, 
        buffer.data(), reinterpret_cast<BITMAPINFO*>(&_bi), DIB_RGB_COLORS);
    return buffer;
}

void Bitmap::setDIBits(const std::vector<BYTE>& data) {
    SetDIBits(_hMemoryDc, _hMemoryBitmap, 0, _size.height,
        data.data(), reinterpret_cast<BITMAPINFO*>(&_bi), DIB_RGB_COLORS);
}

void Bitmap::drawCursor(const Cursor& cursor) const {
    DrawIcon(_hMemoryDc, cursor.point.x, cursor.point.y, cursor.hCursor);
}

Bitmap::~Bitmap() {
    SelectObject(_hMemoryDc, _oldBitmap);
    DeleteObject(_hMemoryBitmap);
    DeleteDC(_hMemoryDc);
}
