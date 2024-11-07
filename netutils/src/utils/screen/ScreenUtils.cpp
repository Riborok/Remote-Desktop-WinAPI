#include "../../../inc/utils/screen/ScreenUtils.hpp"

void ScreenUtils::enableHighQualityStretching(const HDC hdc) {
    SetStretchBltMode(hdc, HALFTONE);
    SetBrushOrgEx(hdc, 0, 0, nullptr);
}

Size ScreenUtils::getScreenSize() {
    return { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
}

Cursor ScreenUtils::getCursor() {
    return { getVisibleHCursor(), getCursorPosition() };
}

HICON ScreenUtils::getVisibleHCursor() {
    CURSORINFO cursorInfo;
    cursorInfo.cbSize = sizeof(CURSORINFO);
    if (GetCursorInfo(&cursorInfo) && cursorInfo.flags == CURSOR_SHOWING) {
        return cursorInfo.hCursor;
    }
    return nullptr;
}

POINT ScreenUtils::getCursorPosition() {
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    return cursorPos;
}
