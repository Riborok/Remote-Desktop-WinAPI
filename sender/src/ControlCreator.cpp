﻿#include "../inc/ControlCreator.hpp"

HWND ControlCreator::createLabel(const int y, const std::wstring& text) const {
    const HWND hLabel = CreateWindow(L"STATIC", text.c_str(), WS_VISIBLE | WS_CHILD, X_OFFSET_LABEL, y, LABEL_WIDTH, LABEL_HEIGHT, _hwnd, nullptr, nullptr, nullptr);
    SendMessage(hLabel, WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hLabelFont), TRUE);
    return hLabel;
}

HWND ControlCreator::createEditControl( const int y, const std::wstring& text) const {
    const HWND hEdit = CreateWindow(L"EDIT", text.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, X_OFFSET_CONTROL, y, CONTROL_WIDTH, CONTROL_HEIGHT, _hwnd, nullptr, nullptr, nullptr);
    SendMessage(hEdit, WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hControlFont), TRUE);
    return hEdit;
}

HWND ControlCreator::createComboBox(const int y, const std::vector<std::wstring>& items, const int selectedIndex) const {
    const HWND hComboBox = CreateWindow(L"COMBOBOX", nullptr, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, X_OFFSET_CONTROL, y, CONTROL_WIDTH, COMBO_BOX_HEIGHT, _hwnd, nullptr, nullptr, nullptr);
    for (const auto& item : items) {
        SendMessage(hComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(item.c_str()));
    }
    SendMessage(hComboBox, CB_SETCURSEL, static_cast<WPARAM>(selectedIndex), 0);
    SendMessage(hComboBox, WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hControlFont), TRUE);
    return hComboBox;
}

HWND ControlCreator::createDefButton(const int y, const std::wstring& text, const int buttonId) const {
    const HWND hButton = CreateWindow(L"BUTTON", text.c_str(), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, X_OFFSET_DEF_BUTTON, y, BUTTON_WIDTH, BUTTON_HEIGHT, _hwnd, reinterpret_cast<HMENU>(buttonId), nullptr, nullptr);
    SendMessage(hButton, WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hLabelFont), TRUE);
    return hButton;
}

HWND ControlCreator::createButton(const int y, const std::wstring& text, const int buttonId) const {
    const HWND hButton = CreateWindow(L"BUTTON", text.c_str(), WS_VISIBLE | WS_CHILD, X_OFFSET_BUTTON, y, BUTTON_WIDTH, BUTTON_HEIGHT, _hwnd, reinterpret_cast<HMENU>(buttonId), nullptr, nullptr);
    SendMessage(hButton, WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hLabelFont), TRUE);
    return hButton;
}