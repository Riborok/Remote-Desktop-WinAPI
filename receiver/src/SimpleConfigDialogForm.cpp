#include "../inc/SimpleConfigDialogForm.hpp"

#include "../resource.h"

SimpleConfigDialogForm::SimpleConfigDialogForm(const HINSTANCE hInstance, const Fonts& fonts):
    _hInstance(hInstance), _fonts(fonts) { }

void SimpleConfigDialogForm::set(const ReceiverConfig& config, Receiver& receiver) {
    _config = config;
    _receiver = &receiver;
}

bool SimpleConfigDialogForm::show(const HWND hWnd) {
    const INT_PTR result = DialogBoxParam(_hInstance, MAKEINTRESOURCE(IDD_SIMPLE_SETTINGS_DIALOG), hWnd, dialogProc, reinterpret_cast<LPARAM>(this));
    return result == IDB_APPLY;
}

LRESULT SimpleConfigDialogForm::dialogProc(const HWND hwndDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
    switch (uMsg) {
        case WM_INITDIALOG: {
            const SimpleConfigDialogForm* dialogForm = reinterpret_cast<SimpleConfigDialogForm*>(lParam);
            SetWindowLongPtr(hwndDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(dialogForm));
            dialogForm->setControlFonts(hwndDlg);
            dialogForm->updateFields(hwndDlg);
            return TRUE;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDB_APPLY: {
                    SimpleConfigDialogForm* dialogForm = reinterpret_cast<SimpleConfigDialogForm*>(GetWindowLongPtr(hwndDlg, GWLP_USERDATA));
                    dialogForm->handleApplyCommand(hwndDlg);
                    EndDialog(hwndDlg, IDB_APPLY);
                    return TRUE;
                }
                case IDB_EXIT:
                    EndDialog(hwndDlg, IDCANCEL);
                    return TRUE;
                }
            break;
        }
        case WM_CLOSE:
            EndDialog(hwndDlg, IDB_CANCEL);
            return TRUE;
    }
    return FALSE;
}

void SimpleConfigDialogForm::setControlFonts(const HWND hwndDlg) const {
    for (const int id : {IDC_FPS_LABEL, IDC_MAX_DELAY_LABEL, IDB_APPLY, IDB_EXIT}) {
        SendMessage(GetDlgItem(hwndDlg, id), WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hLabelFont), TRUE);
    }
        
    for (const int id : {IDC_FPS, IDC_MAX_DELAY}) {
        SendMessage(GetDlgItem(hwndDlg, id), WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hControlFont), TRUE);
    }
}

void SimpleConfigDialogForm::updateFields(const HWND hwndDlg) const {
    SetDlgItemInt(hwndDlg, IDC_FPS, _config.fps, FALSE);
    SetDlgItemInt(hwndDlg, IDC_MAX_DELAY, _config.maxDelayMs, FALSE);
}

void SimpleConfigDialogForm::handleApplyCommand(const HWND hwndDlg) {
    static constexpr size_t BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];

    GetDlgItemTextA(hwndDlg, IDC_FPS, buffer, BUFFER_SIZE);
    _config.fps = std::stoi(buffer);

    GetDlgItemTextA(hwndDlg, IDC_MAX_DELAY, buffer, BUFFER_SIZE);
    _config.maxDelayMs = std::stoi(buffer);

    _receiver->updateFPSAndMaxDelay(_config.fps, _config.maxDelayMs);
}
