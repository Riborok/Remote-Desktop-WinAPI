#pragma once

#include <windows.h>

class MessageUtils {
public:
    MessageUtils() = delete;

    static bool isKeyboardMessage(const UINT message);
    static bool isMouseMessage(const UINT message);
};
