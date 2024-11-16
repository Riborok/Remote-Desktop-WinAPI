#include "../../../inc/screen/capture/CursorCapture.hpp"

std::optional<Icon> CursorCapture::capture() {
    const CURSORINFO cursorInfo = retrieveCursorInfo();
    if (cursorInfo.flags == CURSOR_SHOWING) {
        if (const auto iconData = fetchIconData(cursorInfo.hCursor)) {
            return createCursorIcon(cursorInfo, *iconData);
        }
    }
    return std::nullopt;
}

CURSORINFO CursorCapture::retrieveCursorInfo() {
    CURSORINFO cursorInfo;
    cursorInfo.cbSize = sizeof(CURSORINFO);
    if (!GetCursorInfo(&cursorInfo)) {
        cursorInfo.flags = 0;
    }
    return cursorInfo;
}

Icon CursorCapture::createCursorIcon(const CURSORINFO& cursorInfo, const CachedIconInfo& iconData) {
    return {
        cursorInfo.hCursor,
        {
            cursorInfo.ptScreenPos.x - iconData.hotspot.x,
            cursorInfo.ptScreenPos.y - iconData.hotspot.y
        },
        iconData.size
    };
}

std::optional<CursorCapture::CachedIconInfo> CursorCapture::fetchIconData(const HICON hIcon) {
    const auto it = _cachedIcons.find(hIcon);
    if (it != _cachedIcons.end()) {
        return it->second;
    }
        
    if (const auto iconInfo = extractIconInfo(hIcon)) {
        _cachedIcons[hIcon] = *iconInfo;
        return iconInfo;
    }

    return std::nullopt;
}

std::optional<CursorCapture::CachedIconInfo> CursorCapture::extractIconInfo(const HICON hIcon) {
    ICONINFO iconInfo;
    if (GetIconInfo(hIcon, &iconInfo)) {
        BITMAP bitmap;
        GetObject(iconInfo.hbmColor ? iconInfo.hbmColor : iconInfo.hbmMask, sizeof(bitmap), &bitmap);

        CachedIconInfo result = {
            {static_cast<LONG>(iconInfo.xHotspot), static_cast<LONG>(iconInfo.yHotspot)},
            {bitmap.bmWidth, bitmap.bmHeight}
        };

        releaseIconResources(iconInfo);
        return result;
    }
    return std::nullopt;
}

void CursorCapture::releaseIconResources(const ICONINFO& iconInfo) noexcept {
    DeleteObject(iconInfo.hbmMask);
    DeleteObject(iconInfo.hbmColor);
}
