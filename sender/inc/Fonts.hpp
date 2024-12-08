#pragma once

#include <windows.h>

struct Fonts {
    HFONT hLabelFont;
    HFONT hControlFont;

    explicit Fonts(const int labelFontSize = 20, const int controlFontSize = 16, const LPCWSTR fontName = L"Courier New");

    ~Fonts();

    Fonts(Fonts&&) = delete;
    Fonts& operator=(Fonts&&) = delete;
    Fonts(const Fonts&) = delete;
    Fonts& operator=(const Fonts&) = delete;
};
