#include "../../../inc/screen/bitmap/Bitmap.hpp"

#include "../../../inc/utils/screen/PointScaler.hpp"

Bitmap::Bitmap(const HDC hScreenDc, const HBITMAP hMemoryBitmap, const SIZE& size, const BITMAPINFOHEADER& bi):
    _hMemoryDc(CreateCompatibleDC(hScreenDc)), _hMemoryBitmap(hMemoryBitmap),
    _oldBitmap(SelectObject(_hMemoryDc, _hMemoryBitmap)), _size(size), _bi(bi) { }

void Bitmap::copyFrom(const HDC src) const {
    BitBlt(_hMemoryDc, 0, 0, _size.cx, _size.cy, src, 0, 0, SRCCOPY);
}

void Bitmap::copyTo(const HDC dest) const {
    BitBlt(dest, 0, 0, _size.cx, _size.cy, _hMemoryDc, 0, 0, SRCCOPY);
}

std::vector<byte> Bitmap::getDIBits() {
    std::vector<byte> buffer(_bi.biSizeImage);
    GetDIBits(_hMemoryDc, _hMemoryBitmap, 0, _size.cy, 
        buffer.data(), reinterpret_cast<BITMAPINFO*>(&_bi), DIB_RGB_COLORS);
    return buffer;
}

void Bitmap::setDIBits(const std::vector<byte>& data) const {
    SetDIBits(_hMemoryDc, _hMemoryBitmap, 0, _size.cy,
        data.data(), reinterpret_cast<const BITMAPINFO*>(&_bi), DIB_RGB_COLORS);
}

void Bitmap::drawIcon(const Icon& icon) const {
    DrawIcon(_hMemoryDc, icon.point.x, icon.point.y, icon.hIcon);
}

const SIZE& Bitmap::getSize() const {
    return _size;
}

Bitmap::~Bitmap() {
    releaseResources();
}

Bitmap::Bitmap(Bitmap&& other) noexcept:
        _hMemoryDc(other._hMemoryDc), 
        _hMemoryBitmap(other._hMemoryBitmap), 
        _oldBitmap(other._oldBitmap), 
        _size(other._size),
        _bi(other._bi) {
    other.resetResources();
}

Bitmap& Bitmap::operator=(Bitmap&& other) noexcept {
    if (this != &other) {
        releaseResources();
        
        _hMemoryDc = other._hMemoryDc;
        _hMemoryBitmap = other._hMemoryBitmap;
        _oldBitmap = other._oldBitmap;
        _size = other._size;
        _bi = other._bi;

        other.resetResources();
    }
    return *this;
}

void Bitmap::releaseResources() const noexcept {
    if (_hMemoryDc) {
        SelectObject(_hMemoryDc, _oldBitmap);
        DeleteDC(_hMemoryDc);
    }
    DeleteObject(_hMemoryBitmap);
}

void Bitmap::resetResources() noexcept {
    _hMemoryDc = nullptr;
    _hMemoryBitmap = nullptr;
    _oldBitmap = nullptr;
}
