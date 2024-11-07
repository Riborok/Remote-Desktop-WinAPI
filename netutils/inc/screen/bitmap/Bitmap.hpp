#pragma once

#include <vector>
#include <windows.h>

#include "../Cursor.hpp"
#include "../Size.hpp"

class Bitmap {
    friend class BitmapFactory;
    
    HDC _hMemoryDc;
    HBITMAP _hMemoryBitmap;
    HGDIOBJ _oldBitmap;
    Size _size;
    BITMAPINFOHEADER _bi;

    Bitmap(const HDC hScreenDc, const HBITMAP hMemoryBitmap, const Size& size, const BITMAPINFOHEADER& bi);
public:
    void enableHighQualityStretching() const;
    void copyFrom(const HDC src) const;
    void copyTo(const HDC dest) const;
    void stretchFrom(const Bitmap& src, const DWORD rop = SRCCOPY) const;
    void stretchFrom(const HDC src, const Size& srcSize, const DWORD rop = SRCCOPY) const;
    void stretchTo(const Bitmap& dest, const DWORD rop = SRCCOPY) const;
    void stretchTo(const HDC dest, const Size& destSize, const DWORD rop = SRCCOPY) const;
    std::vector<BYTE> getDIBits();
    void setDIBits(const std::vector<BYTE>& data);
    void drawCursor(const Cursor& cursor) const;

    ~Bitmap();

    Bitmap(Bitmap&&) = delete;
    Bitmap& operator=(Bitmap&&) = delete;
    Bitmap(const Bitmap&) = delete;
    Bitmap& operator=(const Bitmap&) = delete;
};
