﻿#pragma once

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
    void stretchedCopyFrom(const HDC src, const SIZE& srcSize) const;
    void stretchedCopyTo(const HDC dest, const SIZE& destSize) const;
    std::vector<byte> getDIBits();
    void setDIBits(const std::vector<byte>& data) const;
    void drawIcon(const Icon& icon) const;
    void drawScaledIcon(const Icon& icon, const SIZE& iconSourceSize) const;
    const SIZE& getSize() const;

    ~Bitmap();

    Bitmap(Bitmap&&) noexcept;
    Bitmap& operator=(Bitmap&&) noexcept;
    
    Bitmap(const Bitmap&) = delete;
    Bitmap& operator=(const Bitmap&) = delete;
private:
    void releaseResources() const noexcept;
    void resetResources() noexcept;
};
