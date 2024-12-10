#include "../inc/MainForm.hpp"

ACCEL MainForm::_accelTableEntries[] = {
    {FALT | FVIRTKEY, VK_F1, ID_MENU},
    {FALT | FVIRTKEY, VK_F11, ID_TOGGLE_FULLSCREEN},
};

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
        _fonts(fonts), _hwnd(createHwnd()), _configDialog(_hInstance, _fonts),
        _hAccel(CreateAcceleratorTable(_accelTableEntries, ARRAYSIZE(_accelTableEntries))){ }

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
        if (!TranslateAccelerator(_hwnd, _hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
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
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case ID_MENU: {
                    MainForm* mainForm = reinterpret_cast<MainForm*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
                    if (!mainForm->_configDialog.show(hwnd)) {
                        PostMessage(hwnd, WM_CLOSE, 0, 0);
                    }
                    break;
                }
                case ID_TOGGLE_FULLSCREEN: {
                    MainForm* mainForm = reinterpret_cast<MainForm*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
                    mainForm->toggleFullscreen();
                    break;
                }
                }
            break;
        }
        case WM_DESTROY: {
            const MainForm* mainForm = reinterpret_cast<MainForm*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            mainForm->_receiver.stop();
            PostQuitMessage(0);
            break;
        }
        default: {
            const MainForm* mainForm = reinterpret_cast<MainForm*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            if (mainForm != nullptr) {
                return mainForm->handleInput(hwnd, uMsg, wParam, lParam);
            }
            break;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT MainForm::handleInput(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) const {
    if (hwnd == _hwnd) {
        switch (uMsg) {
            case WM_SIZE: {
                if (wParam != SIZE_MINIMIZED) {
                    const MainForm* mainForm = reinterpret_cast<MainForm*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
                    mainForm->_receiver.updateAppSize({LOWORD(lParam), HIWORD(lParam)});
                }
                break;
            }
            case WM_MOUSEMOVE:
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
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void MainForm::toggleFullscreen() {
    if (_isFullscreen) {
        SetWindowLong(_hwnd, GWL_STYLE, _hwndStyle);
        SetWindowLong(_hwnd, GWL_EXSTYLE, _hwndStyleEx);
        SetWindowPlacement(_hwnd, &_wpc);
        ShowWindow(_hwnd, SW_SHOWNORMAL);
    } else {
        GetWindowPlacement(_hwnd, &_wpc);
        if (_hwndStyle == 0) _hwndStyle = GetWindowLong(_hwnd, GWL_STYLE);
        if (_hwndStyleEx == 0) _hwndStyleEx = GetWindowLong(_hwnd, GWL_EXSTYLE);
        const LONG newHwndStyle = _hwndStyle & ~WS_BORDER & ~WS_DLGFRAME & ~WS_THICKFRAME;
        const LONG newHwndStyleEx = _hwndStyleEx & ~WS_EX_WINDOWEDGE;
        SetWindowLong(_hwnd, GWL_STYLE, newHwndStyle | WS_POPUP);
        SetWindowLong(_hwnd, GWL_EXSTYLE, newHwndStyleEx | WS_EX_TOPMOST);
        ShowWindow(_hwnd, SW_SHOWMAXIMIZED);
    }
    _isFullscreen = !_isFullscreen;
}

MainForm::~MainForm() {
    DestroyAcceleratorTable(_hAccel);
}
