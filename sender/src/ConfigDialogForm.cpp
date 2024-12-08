#include "../inc/ConfigDialogForm.hpp"

#include <map>

#include "../resource.h"
#include "../inc/RegistrySettings.hpp"

ConfigDialogForm::ConfigDialogForm(const HINSTANCE hInstance, const Fonts& fonts):
    _hInstance(hInstance), _fonts(fonts) { }

bool ConfigDialogForm::show(SenderCfg& cfg) {
    _config = &cfg;
    const INT_PTR result = DialogBoxParam(_hInstance, MAKEINTRESOURCE(IDD_SETTINGS_DIALOG), nullptr, dialogProc, reinterpret_cast<LPARAM>(this));
    return result == IDOK;
}

LRESULT ConfigDialogForm::dialogProc(const HWND hwndDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
    static std::map<HWND, const ConfigDialogForm*> configDialogForms;
    switch (uMsg) {
        case WM_INITDIALOG: {
            const ConfigDialogForm* configDialogForm = reinterpret_cast<const ConfigDialogForm*>(lParam);
            configDialogForms[hwndDlg] = configDialogForm;
            configDialogForm->setControlFonts(hwndDlg);
            configDialogForm->loadSettingsAndUpdateFields(hwndDlg);
            return TRUE;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDB_OK: {
                    const ConfigDialogForm* configDialogForm = configDialogForms[hwndDlg];
                    configDialogForm->handleOkCommand(hwndDlg);
                    EndDialog(hwndDlg, IDOK);
                    return TRUE;
                }
                case IDB_CANCEL:
                    EndDialog(hwndDlg, IDCANCEL);
                    return TRUE;
            }
            break;
        }
        case WM_CLOSE:
            EndDialog(hwndDlg, IDCANCEL);
            return TRUE;
        case WM_DESTROY:
            configDialogForms.erase(hwndDlg);
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

void ConfigDialogForm::handleOkCommand(const HWND hwndDlg) const {
    static constexpr size_t BUFFER_SIZE = 256;
    wchar_t buffer[BUFFER_SIZE];

    GetDlgItemText(hwndDlg, IDC_PORT, buffer, BUFFER_SIZE);
    _config->tcpServerPort = static_cast<u_short>(std::stoi(buffer));

    GetDlgItemText(hwndDlg, IDC_FPS, buffer, BUFFER_SIZE);
    _config->fps = std::stoi(buffer);

    GetDlgItemText(hwndDlg, IDC_DELAY, buffer, BUFFER_SIZE);
    _config->maxDelayMs = std::stoi(buffer);

    GetDlgItemText(hwndDlg, IDC_QUALITY, buffer, BUFFER_SIZE);
    _config->imageConfig.quality = std::stoi(buffer);

    GetDlgItemText(hwndDlg, IDC_WIDTH, buffer, BUFFER_SIZE);
    _config->targetSize.cx = std::stoi(buffer);

    GetDlgItemText(hwndDlg, IDC_HEIGHT, buffer, BUFFER_SIZE);
    _config->targetSize.cy = std::stoi(buffer);

    const HWND hComboBox = GetDlgItem(hwndDlg, IDC_FORMAT);
    const int selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
    _config->imageConfig.ext = static_cast<ImageFormat>(selectedIndex);

    RegistrySettings::saveSettingsToRegistry(*_config);
}
