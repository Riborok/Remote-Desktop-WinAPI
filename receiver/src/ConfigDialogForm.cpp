﻿#include "utils/sock/SockaddrUtils.hpp"
#include "../inc/ConfigDialogForm.hpp"

#include <stdexcept>

#include "../resource.h"
#include "../inc/RegistrySettings.hpp"

ConfigDialogForm::ConfigDialogForm(const HINSTANCE hInstance, const Fonts& fonts)
    : _hInstance(hInstance), _fonts(fonts) { }

bool ConfigDialogForm::show(ReceiverConfig& config) {
    _config = &config;
    const INT_PTR result = DialogBoxParam(
        _hInstance, 
        MAKEINTRESOURCE(IDD_SETTINGS_DIALOG), 
        nullptr, 
        dialogProc, 
        reinterpret_cast<LPARAM>(this)
    );
    return result == IDB_OK;
}

LRESULT CALLBACK ConfigDialogForm::dialogProc(const HWND hwndDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
    switch (uMsg) {
        case WM_INITDIALOG: {
            const ConfigDialogForm* dialogForm = reinterpret_cast<ConfigDialogForm*>(lParam);
            SetWindowLongPtr(hwndDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(dialogForm));
            dialogForm->setControlFonts(hwndDlg);
            dialogForm->loadSettingsAndUpdateFields(hwndDlg);
            return TRUE;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDB_OK: {
                    const ConfigDialogForm* dialogForm = reinterpret_cast<ConfigDialogForm*>(GetWindowLongPtr(hwndDlg, GWLP_USERDATA));
                    if (dialogForm->handleOkCommand(hwndDlg)) {
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
    for (const int id : {IDC_SERVER_IP_LABEL, IDC_SERVER_PORT_LABEL, IDC_UDP_PORT_LABEL, IDC_FPS_LABEL, IDC_MAX_DELAY_LABEL, IDB_OK, IDB_CANCEL}) {
        SendMessage(GetDlgItem(hwndDlg, id), WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hLabelFont), TRUE);
    }
        
    for (const int id : {IDC_SERVER_IP, IDC_SERVER_PORT, IDC_UDP_PORT, IDC_FPS, IDC_MAX_DELAY}) {
        SendMessage(GetDlgItem(hwndDlg, id), WM_SETFONT, reinterpret_cast<WPARAM>(_fonts.hControlFont), TRUE);
    }
}

void ConfigDialogForm::loadSettingsAndUpdateFields(const HWND hwndDlg) const {
    RegistrySettings::loadSettingsFromRegistry(*_config);
    
    SetDlgItemTextA(hwndDlg, IDC_SERVER_IP, SockaddrUtils::ipToStr(_config->serverIp).c_str());
    SetDlgItemInt(hwndDlg, IDC_SERVER_PORT, _config->serverPort, FALSE);
    SetDlgItemInt(hwndDlg, IDC_UDP_PORT, _config->udpPort, FALSE);
    SetDlgItemInt(hwndDlg, IDC_FPS, _config->fps, FALSE);
    SetDlgItemInt(hwndDlg, IDC_MAX_DELAY, _config->maxDelayMs, FALSE);
}

bool ConfigDialogForm::handleOkCommand(const HWND hwndDlg) const {
    static constexpr size_t BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];
    GetDlgItemTextA(hwndDlg, IDC_SERVER_IP, buffer, BUFFER_SIZE);
    try {
        _config->serverIp = SockaddrUtils::strToIp(buffer);   
    } catch (const std::runtime_error&) {
        MessageBox(hwndDlg, L"Invalid IP address format.", L"Error", MB_ICONERROR);
        return false;
    }

    _config->serverPort = static_cast<u_short>(GetDlgItemInt(hwndDlg, IDC_SERVER_PORT, nullptr, FALSE));
    _config->udpPort = static_cast<u_short>(GetDlgItemInt(hwndDlg, IDC_UDP_PORT, nullptr, FALSE));
    _config->fps = GetDlgItemInt(hwndDlg, IDC_FPS, nullptr, FALSE);
    _config->maxDelayMs = GetDlgItemInt(hwndDlg, IDC_MAX_DELAY, nullptr, FALSE);

    RegistrySettings::saveSettingsToRegistry(*_config);
    return true;
}