#pragma once

#include <windows.h>
#include <string>

#include "./remote-desk/receiver/ReceiverConfig.hpp"
#include "./Fonts.hpp"

class ReceiverConfigDialogForm {
    HINSTANCE _hInstance;
    ReceiverConfig* _config = nullptr;
    const Fonts& _fonts;

public:
    ReceiverConfigDialogForm(const HINSTANCE hInstance, const Fonts& fonts);
    bool show(ReceiverConfig& config);

    ~ReceiverConfigDialogForm() = default;
    ReceiverConfigDialogForm(ReceiverConfigDialogForm&&) = delete;
    ReceiverConfigDialogForm& operator=(ReceiverConfigDialogForm&&) = delete;
    ReceiverConfigDialogForm(const ReceiverConfigDialogForm&) = delete;
    ReceiverConfigDialogForm& operator=(const ReceiverConfigDialogForm&) = delete;

private:
    static LRESULT CALLBACK dialogProc(const HWND hwndDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
    void setControlFonts(const HWND hwndDlg) const;
    void loadSettingsAndUpdateFields(const HWND hwndDlg) const;
    bool handleOkCommand(const HWND hwndDlg) const;
};
