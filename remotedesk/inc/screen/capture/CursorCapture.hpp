#pragma once

#include <map>
#include <optional>
#include <windows.h>

#include "../Icon.hpp"

class CursorCapture {
    struct CachedIconInfo {
        POINT hotspot;
        SIZE size;
    };
    
    std::map<HICON, CachedIconInfo> _cachedIcons;
public:
    std::optional<Icon> capture();
private:
    static CURSORINFO retrieveCursorInfo();
    static Icon createCursorIcon(const CURSORINFO& cursorInfo, const CachedIconInfo& iconData);
    std::optional<CachedIconInfo> fetchIconData(const HICON hIcon);
    static std::optional<CachedIconInfo> extractIconInfo(const HICON hIcon);
    static void releaseIconResources(const ICONINFO& iconInfo) noexcept;
};
