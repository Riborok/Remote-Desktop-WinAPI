#pragma once

#include <map>
#include <optional>
#include <windows.h>

#include "../Icon.hpp"

class CursorCapture {
    std::map<HICON, POINT> _cachedHotspots;
public:
    std::optional<Icon> capture();
private:
    static CURSORINFO getCursorInfo();
    static Icon createCursorIcon(const CURSORINFO& cursorInfo, const POINT& hotspot);
    POINT* getHotspotFromCache(const HICON hIcon);
    static std::optional<POINT> getHotspotFromIconInfo(const HICON hIcon);
    static void releaseIconInfoResources(const ICONINFO& iconInfo) noexcept;
};
