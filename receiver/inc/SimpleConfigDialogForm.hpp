#pragma once

#include "remote-desk/receiver/Receiver.hpp"
#include "Fonts.hpp"

class SimpleConfigDialogForm {
    HINSTANCE _hInstance;
    ReceiverConfig _config = {};
    Receiver* _receiver = nullptr;
    const Fonts& _fonts;
public:
    SimpleConfigDialogForm(const HINSTANCE hInstance, const Fonts& fonts);
    void set(const ReceiverConfig& config, Receiver& receiver);
    bool show(const HWND hWnd);

    ~SimpleConfigDialogForm() = default;
    SimpleConfigDialogForm(SimpleConfigDialogForm&&) = delete;
    SimpleConfigDialogForm& operator=(SimpleConfigDialogForm&&) = delete;
    SimpleConfigDialogForm(const SimpleConfigDialogForm&) = delete;
    SimpleConfigDialogForm& operator=(const SimpleConfigDialogForm&) = delete;
private:
    static LRESULT CALLBACK dialogProc(const HWND hwndDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
    void setControlFonts(const HWND hwndDlg) const;
    void updateFields(const HWND hwndDlg) const;
    void handleApplyCommand(const HWND hwndDlg);
};
