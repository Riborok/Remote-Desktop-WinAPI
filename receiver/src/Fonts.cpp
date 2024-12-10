#include "../inc/Fonts.hpp"

Fonts::Fonts(const int labelFontSize, const int controlFontSize, const LPCWSTR fontName):
    hLabelFont(CreateFont(labelFontSize, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FF_SWISS, fontName)),
    hControlFont(CreateFont(controlFontSize, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FF_SWISS, fontName)) { }

Fonts::~Fonts() {
    DeleteObject(hLabelFont);
    DeleteObject(hControlFont);
}
