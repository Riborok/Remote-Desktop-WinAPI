#pragma once

#include "../../screen/Icon.hpp"

class ScreenUtils {
public:
    ScreenUtils() = delete;

    static void enableHighQualityStretching(const HDC hdc);
    static SIZE getScreenSize();
};
