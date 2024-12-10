#pragma once

#include <string>
#include <vector>

#include "Fonts.hpp"

class ControlCreator {
    const HWND _hwnd;
    const Fonts& _fonts;
public:
    static constexpr int LABEL_WIDTH = 220;
    static constexpr int CONTROL_WIDTH = 298;
    static constexpr int LABEL_HEIGHT = 20;
    static constexpr int CONTROL_HEIGHT = 26;
    static constexpr int COMBO_BOX_HEIGHT = 100;
    static constexpr int BUTTON_WIDTH = 148;
    static constexpr int BUTTON_HEIGHT = 28;

    static constexpr int X_OFFSET_LABEL = 23;
    static constexpr int X_OFFSET_CONTROL = 243;
    static constexpr int Y_OFFSET = 38;
    static constexpr int BUTTON_Y_OFFSET = 20;
    static constexpr int X_OFFSET_DEF_BUTTON = 120;
    static constexpr int X_OFFSET_BUTTON = 300;
    
    explicit ControlCreator(const HWND hwnd, const Fonts& fonts): _hwnd(hwnd), _fonts(fonts) {}

    HWND createLabel(const int y, const std::wstring& text) const;
    HWND createEditControl(const int y, const std::wstring& text) const;
    HWND createComboBox(const int y, const std::vector<std::wstring>& items, const int selectedIndex) const;
    HWND createDefButton(const int y, const std::wstring& text, const int buttonId) const;
    HWND createButton(const int y, const std::wstring& text, const int buttonId) const;
    
    ~ControlCreator() = default;
    ControlCreator(ControlCreator&&) = delete;
    ControlCreator& operator=(ControlCreator&&) = delete;
    ControlCreator(const ControlCreator&) = delete;
    ControlCreator& operator=(const ControlCreator&) = delete;
};
