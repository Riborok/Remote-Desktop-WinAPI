#include "../inc/ControlCreator.hpp"

HWND ControlCreator::createLabel(const int y, const std::wstring& text) const {
    const HWND hLabel = CreateWindow(L"STATIC", text.c_str(), WS_VISIBLE | WS_CHILD, xOffsetLabel, y, labelWidth, labelHeight, _hwnd, nullptr, nullptr, nullptr);
    SendMessage(hLabel, WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hLabelFont), TRUE);
    return hLabel;
}

HWND ControlCreator::createEditControl( const int y, const std::wstring& text) const {
    const HWND hEdit = CreateWindow(L"EDIT", text.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, xOffsetControl, y, controlWidth, controlHeight, _hwnd, nullptr, nullptr, nullptr);
    SendMessage(hEdit, WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hControlFont), TRUE);
    return hEdit;
}

HWND ControlCreator::createComboBox(const int y, const std::vector<std::wstring>& items, const int selectedIndex) const {
    const HWND hComboBox = CreateWindow(L"COMBOBOX", nullptr, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, xOffsetControl, y, controlWidth, comboBoxHeight, _hwnd, nullptr, nullptr, nullptr);
    for (const auto& item : items) {
        SendMessage(hComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(item.c_str()));
    }
    SendMessage(hComboBox, CB_SETCURSEL, static_cast<WPARAM>(selectedIndex), 0);
    SendMessage(hComboBox, WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hControlFont), TRUE);
    return hComboBox;
}

HWND ControlCreator::createCenteredButton(const int y, const std::wstring& text, const int buttonId) const {
    RECT clientRect;
    GetClientRect(_hwnd, &clientRect);
    const int clientWidth = clientRect.right - clientRect.left;
    const int x = (clientWidth - buttonWidth) / 2;

    const HWND hButton = CreateWindow(L"BUTTON", text.c_str(), WS_VISIBLE | WS_CHILD, x, y, buttonWidth, buttonHeight, _hwnd, reinterpret_cast<HMENU>(buttonId), nullptr, nullptr);
    SendMessage(hButton, WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hLabelFont), TRUE);
    return hButton;
}
