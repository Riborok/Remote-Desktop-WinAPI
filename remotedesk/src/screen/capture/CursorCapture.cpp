#include "../../../inc/screen/capture/CursorCapture.hpp"

std::optional<Icon> CursorCapture::capture() {
    const CURSORINFO cursorInfo = getCursorInfo();
    if (cursorInfo.flags == CURSOR_SHOWING) {
        if (const POINT* hotspot = getHotspotFromCache(cursorInfo.hCursor)) {
            return createCursorIcon(cursorInfo, *hotspot);
        }
    }
    return std::nullopt;
}

CURSORINFO CursorCapture::getCursorInfo() {
    CURSORINFO cursorInfo;
    cursorInfo.cbSize = sizeof(CURSORINFO);
    if (!GetCursorInfo(&cursorInfo)) {
        cursorInfo.flags = 0;
    }
    return cursorInfo;
}

Icon CursorCapture::createCursorIcon(const CURSORINFO& cursorInfo, const POINT& hotspot) {
    return {
        cursorInfo.hCursor,
        {
            cursorInfo.ptScreenPos.x - hotspot.x,
            cursorInfo.ptScreenPos.y - hotspot.y
        }
    };
}

POINT* CursorCapture::getHotspotFromCache(const HICON hIcon) {
    const auto it = _cachedHotspots.find(hIcon);
    if (it != _cachedHotspots.end()) {
        return &it->second;
    }
    
    if (const auto hotspot = getHotspotFromIconInfo(hIcon)) {
        return &_cachedHotspots.emplace(hIcon, *hotspot).first->second;
    }

    return nullptr;
}

std::optional<POINT> CursorCapture::getHotspotFromIconInfo(const HICON hIcon) {
    ICONINFO iconInfo;
    if (GetIconInfo(hIcon, &iconInfo)) {
        releaseIconInfoResources(iconInfo);
        return POINT{static_cast<LONG>(iconInfo.xHotspot), static_cast<LONG>(iconInfo.yHotspot)};
    }
    return std::nullopt;
}

void CursorCapture::releaseIconInfoResources(const ICONINFO& iconInfo) noexcept {
    DeleteObject(iconInfo.hbmMask);
    DeleteObject(iconInfo.hbmColor);
}
