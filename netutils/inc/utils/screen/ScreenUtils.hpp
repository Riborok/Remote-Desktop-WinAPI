#pragma once

#include "../../screen/Cursor.hpp"
#include "../../screen/Size.hpp"

class ScreenUtils {
public:
    ScreenUtils() = delete;

    static void enableHighQualityStretching(const HDC hdc);
    static Size getScreenSize();
    static Cursor getCursor();
private:
    static HICON getVisibleHCursor();
    static POINT getCursorPosition();
};
