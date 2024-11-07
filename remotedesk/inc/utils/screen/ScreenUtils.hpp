#pragma once

#include "../../screen/Icon.hpp"

class ScreenUtils {
public:
    ScreenUtils() = delete;

    static void enableHighQualityStretching(const HDC hdc);
    static SIZE getScreenSize();
    static Icon getCursor();
private:
    static HICON getVisibleHCursor();
    static POINT getCursorPosition();
};
