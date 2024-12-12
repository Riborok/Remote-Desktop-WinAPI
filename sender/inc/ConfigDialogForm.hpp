#pragma once

#include <string>

#include "./Fonts.hpp"
#include "remote-desk/sender/SenderConfig.hpp"

class ConfigDialogForm {
    HINSTANCE _hInstance;
    SenderConfig* _config = nullptr;
    const Fonts& _fonts;
public:
    ConfigDialogForm(const HINSTANCE hInstance, const Fonts& fonts);
    bool show(SenderConfig& config);

    ~ConfigDialogForm() = default;
    ConfigDialogForm(ConfigDialogForm&&) = delete;
    ConfigDialogForm& operator=(ConfigDialogForm&&) = delete;
    ConfigDialogForm(const ConfigDialogForm&) = delete;
    ConfigDialogForm& operator=(const ConfigDialogForm&) = delete;
private:
    static LRESULT CALLBACK dialogProc(const HWND hwndDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
    void setControlFonts(const HWND hwndDlg) const;
    void loadSettingsAndUpdateFields(const HWND hwndDlg) const;
    bool handleOkCommand(const HWND hwndDlg) const;
};
