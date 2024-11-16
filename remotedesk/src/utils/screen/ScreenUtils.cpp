#include "../../../inc/utils/screen/ScreenUtils.hpp"

void ScreenUtils::enableHighQualityStretching(const HDC hdc) {
    SetStretchBltMode(hdc, HALFTONE);
    SetBrushOrgEx(hdc, 0, 0, nullptr);
}

SIZE ScreenUtils::getScreenSize() {
    return { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
}
