#pragma once

#include <vector>
#include <windows.h>

#include "../Icon.hpp"

class Bitmap {
    friend class BitmapFactory;
    
    HDC _hMemoryDc;
    HBITMAP _hMemoryBitmap;
    HGDIOBJ _oldBitmap;
    SIZE _size;
    BITMAPINFOHEADER _bi;

    Bitmap(const HDC hScreenDc, const HBITMAP hMemoryBitmap, const SIZE& size, const BITMAPINFOHEADER& bi);
public:
    void enableHighQualityStretching() const;
    void copyFrom(const HDC src) const;
    void copyTo(const HDC dest) const;
    void stretchFrom(const Bitmap& src, const DWORD rop = SRCCOPY) const;
    void stretchFrom(const HDC src, const SIZE& srcSize, const DWORD rop = SRCCOPY) const;
    void stretchTo(const Bitmap& dest, const DWORD rop = SRCCOPY) const;
    void stretchTo(const HDC dest, const SIZE& destSize, const DWORD rop = SRCCOPY) const;
    std::vector<byte> getDIBits();
    void setDIBits(const std::vector<byte>& data) const;
    void drawIcon(const Icon& icon) const;
    const SIZE& getSize() const;

    ~Bitmap();

    Bitmap(Bitmap&&) noexcept;
    Bitmap& operator=(Bitmap&&) noexcept;
    void releaseResources() const noexcept;
    void resetResources() noexcept;

    Bitmap(const Bitmap&) = delete;
    Bitmap& operator=(const Bitmap&) = delete;
};
