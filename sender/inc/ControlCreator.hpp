#pragma once

#include <string>
#include <vector>

#include "Fonts.hpp"

class ControlCreator {
    const HWND _hwnd;
    const Fonts& _fonts;
public:
    static constexpr int labelWidth = 220;
    static constexpr int controlWidth = 298;
    static constexpr int labelHeight = 20;
    static constexpr int controlHeight = 26;
    static constexpr int comboBoxHeight = 100;
    static constexpr int xOffsetLabel = 23;
    static constexpr int xOffsetControl = 243;
    static constexpr int yOffset = 38;
    static constexpr int buttonYOffset = 20;
    static constexpr int buttonWidth = 148;
    static constexpr int buttonHeight = 28;
    
    explicit ControlCreator(const HWND hwnd, const Fonts& fonts): _hwnd(hwnd), _fonts(fonts) {}

    HWND createLabel(const int y, const std::wstring& text) const;
    HWND createEditControl(const int y, const std::wstring& text) const;
    HWND createComboBox(const int y, const std::vector<std::wstring>& items, const int selectedIndex) const;
    HWND createCenteredButton(const int y, const std::wstring& text, const int buttonId) const;
    
    ~ControlCreator() = default;
    ControlCreator(ControlCreator&&) = delete;
    ControlCreator& operator=(ControlCreator&&) = delete;
    ControlCreator(const ControlCreator&) = delete;
    ControlCreator& operator=(const ControlCreator&) = delete;
};
