#pragma once

#include <windows.h>

class DC {
    HWND _hWnd;
    HDC _hDc;
public:
    explicit DC(const HWND hWnd);
    HDC getHDC() const;
    ~DC();

    DC(DC&&) = delete;
    DC& operator=(DC&&) = delete;
    DC(const DC&) = delete;
    DC& operator=(const DC&) = delete;
};
