#include "../inc/MainForm.hpp"

HINSTANCE MainForm::_hInstance = nullptr;

void MainForm::registerClass(const HINSTANCE hInstance) {
    _hInstance = hInstance;
    
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = windowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WINDOWS_CLASS_NAME;
    RegisterClassEx(&wc);
}

MainForm::MainForm(const Fonts& fonts):
        _fonts(fonts), _hwnd(createHwnd()), _configDialog(_hInstance, _fonts) { }

HWND MainForm::createHwnd() {
    return CreateWindowEx(0, WINDOWS_CLASS_NAME, L"TCP Screenshot Receiver",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        nullptr, nullptr, _hInstance, this);
}

void MainForm::set(const ReceiverConfig& config, Receiver&& receiver) {
    _config = config;
    _receiver = std::move(receiver);
    _receiver.setDisconnectCallback([this]{ onConnectionClosed(); });
    _configDialog.set(_config, _receiver);
}

void MainForm::onConnectionClosed() const {
    MessageBox(_hwnd, L"Connection closed.", L"Info", MB_OK | MB_ICONINFORMATION);
    PostMessage(_hwnd, WM_CLOSE, 0, 0);
}

void MainForm::show() const {
    ShowWindow(_hwnd, SW_SHOW);
    _receiver.run();
    
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HWND MainForm::getHwnd() const { return _hwnd; }

LRESULT MainForm::windowProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            const CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            MainForm* mainForm = static_cast<MainForm*>(pCreateStruct->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(mainForm));
            break;
        }

        case WM_SIZE: {
            if (wParam != SIZE_MINIMIZED) {
                const MainForm* mainForm = reinterpret_cast<MainForm*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
                mainForm->_receiver.updateAppSize({LOWORD(lParam), HIWORD(lParam)});
            }
            break;
        }
        //case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP: {
            const MainForm* mainForm = reinterpret_cast<MainForm*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            mainForm->_receiver.sendEvent(uMsg, wParam, lParam);
            return 0;
        }
        
        case WM_DESTROY: {
            const MainForm* mainForm = reinterpret_cast<MainForm*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            mainForm->_receiver.stop();
            PostQuitMessage(0);
            break;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
