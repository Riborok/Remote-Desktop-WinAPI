#include "utils/sock/SockaddrUtils.hpp"
#include "../inc/ConfigDialogForm.hpp"

#include "../resource.h"
#include "../inc/RegistrySettings.hpp"
#include "../inc/utils.hpp"

ConfigDialogForm::ConfigDialogForm(const HINSTANCE hInstance, const Fonts& fonts):
    _hInstance(hInstance), _fonts(fonts) { }

bool ConfigDialogForm::show(SenderConfig& config) {
    _config = &config;
    const INT_PTR result = DialogBoxParam(_hInstance, MAKEINTRESOURCE(IDD_SETTINGS_DIALOG), nullptr, dialogProc, reinterpret_cast<LPARAM>(this));
    return result == IDB_OK;
}

LRESULT ConfigDialogForm::dialogProc(const HWND hwndDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
    switch (uMsg) {
        case WM_INITDIALOG: {
            const ConfigDialogForm* configDialogForm = reinterpret_cast<ConfigDialogForm*>(lParam);
            SetWindowLongPtr(hwndDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(configDialogForm));
            configDialogForm->setControlFonts(hwndDlg);
            configDialogForm->loadSettingsAndUpdateFields(hwndDlg);
            return TRUE;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDB_OK: {
                    const ConfigDialogForm* configDialogForm = reinterpret_cast<ConfigDialogForm*>(GetWindowLongPtr(hwndDlg, GWLP_USERDATA));
                    if (configDialogForm->handleOkCommand(hwndDlg)) {
                        EndDialog(hwndDlg, IDB_OK);   
                    }
                    return TRUE;
                }
                case IDB_CANCEL:
                    EndDialog(hwndDlg, IDB_CANCEL);
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

void ConfigDialogForm::setControlFonts(const HWND hwndDlg) const {
    for (const int id : {IDC_PORT_LABEL, IDC_FPS_LABEL, IDC_DELAY_LABEL, IDC_QUALITY_LABEL, IDC_WIDTH_LABEL, IDC_HEIGHT_LABEL, IDC_FORMAT_LABEL, IDB_OK, IDB_CANCEL}) {
        SendMessage(GetDlgItem(hwndDlg, id), WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hLabelFont), TRUE);
    }
        
    for (const int id : {IDC_PORT, IDC_FPS, IDC_DELAY, IDC_QUALITY, IDC_WIDTH, IDC_HEIGHT, IDC_FORMAT}) {
        SendMessage(GetDlgItem(hwndDlg, id), WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hControlFont), TRUE);
    }
}

void ConfigDialogForm::loadSettingsAndUpdateFields(const HWND hwndDlg) const {
    RegistrySettings::loadSettingsFromRegistry(*_config);

    SetDlgItemInt(hwndDlg, IDC_PORT, _config->tcpServerPort, FALSE);
    SetDlgItemInt(hwndDlg, IDC_FPS, _config->fps, FALSE);
    SetDlgItemInt(hwndDlg, IDC_DELAY, _config->maxDelayMs, FALSE);
    SetDlgItemInt(hwndDlg, IDC_QUALITY, _config->imageConfig.quality, FALSE);
    SetDlgItemInt(hwndDlg, IDC_WIDTH, _config->targetSize.cx, FALSE);
    SetDlgItemInt(hwndDlg, IDC_HEIGHT, _config->targetSize.cy, FALSE);

    const HWND hComboBox = GetDlgItem(hwndDlg, IDC_FORMAT);
    SendMessage(hComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"jpg"));
    SendMessage(hComboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"webp"));
    SendMessage(hComboBox, CB_SETCURSEL, static_cast<WPARAM>(_config->imageConfig.ext), 0);
}

bool ConfigDialogForm::handleOkCommand(const HWND hwndDlg) const {
   static constexpr size_t BUFFER_SIZE = 256;
    wchar_t buffer[BUFFER_SIZE];

    int tempValue;
    std::wstring accumulatedErrors;

    GetDlgItemText(hwndDlg, IDC_PORT, buffer, BUFFER_SIZE);
    if (!safeStoi(buffer, tempValue)) {
        accumulatedErrors += L"Invalid port number.\n";
    } else {
        _config->tcpServerPort = static_cast<u_short>(tempValue);
        if (SockaddrUtils::isPortInUse(_config->tcpServerPort)) {
            accumulatedErrors += L"Invalid UDP port: the specified port is already in use.\n";
        }
    }

    GetDlgItemText(hwndDlg, IDC_FPS, buffer, BUFFER_SIZE);
    if (!safeStoi(buffer, tempValue)) {
        accumulatedErrors += L"Invalid FPS value.\n";
    } else {
        _config->fps = tempValue;
    }

    GetDlgItemText(hwndDlg, IDC_DELAY, buffer, BUFFER_SIZE);
    if (!safeStoi(buffer, tempValue)) {
        accumulatedErrors += L"Invalid delay value.\n";
    } else {
        _config->maxDelayMs = tempValue;
    }

    GetDlgItemText(hwndDlg, IDC_QUALITY, buffer, BUFFER_SIZE);
    if (!safeStoi(buffer, tempValue) || tempValue < 1 || tempValue > 100) {
        accumulatedErrors += L"Invalid quality value.\n";
    } else {
        _config->imageConfig.quality = tempValue;
    }

    GetDlgItemText(hwndDlg, IDC_WIDTH, buffer, BUFFER_SIZE);
    if (!safeStoi(buffer, tempValue)) {
        accumulatedErrors += L"Invalid width value.\n";
    } else {
        _config->targetSize.cx = tempValue;
    }

    GetDlgItemText(hwndDlg, IDC_HEIGHT, buffer, BUFFER_SIZE);
    if (!safeStoi(buffer, tempValue)) {
        accumulatedErrors += L"Invalid height value.\n";
    } else {
        _config->targetSize.cy = tempValue;
    }

    const HWND hComboBox = GetDlgItem(hwndDlg, IDC_FORMAT);
    const int selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
    _config->imageConfig.ext = static_cast<ImageFormat>(selectedIndex);

    if (!accumulatedErrors.empty()) {
        MessageBox(hwndDlg, std::wstring(accumulatedErrors.begin(), accumulatedErrors.end()).c_str(), L"Error", MB_ICONERROR);
        return false;
    }

    RegistrySettings::saveSettingsToRegistry(*_config);
    return true;
}
