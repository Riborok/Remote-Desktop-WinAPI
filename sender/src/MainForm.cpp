#include "../inc/MainForm.hpp"

#include "../inc/ControlCreator.hpp"

HINSTANCE MainForm::_hInstance = nullptr;

void MainForm::registerClass(const HINSTANCE hInstance) {
    _hInstance = hInstance;
    
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = windowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WINDOWS_CLASS_NAME;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    RegisterClassEx(&wc);
}

MainForm::MainForm(const Fonts& fonts, const SenderConfig& config):
        _fonts(fonts),
        _config(config),
        _hwnd(createHwnd()),
        _sender(_config) {
    _sender.setDisconnectCallback([this]{ onConnectionClosed(); });
}

void MainForm::onConnectionClosed() const {
    MessageBox(_hwnd, L"Connection closed.", L"Info", MB_OK | MB_ICONINFORMATION);
    PostMessage(_hwnd, WM_CLOSE, 0, 0);
}

HWND MainForm::createHwnd() {
    return CreateWindowEx(0, WINDOWS_CLASS_NAME, L"TCP Screenshot Sender",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        (GetSystemMetrics(SM_CXSCREEN) - 586) / 2, (GetSystemMetrics(SM_CYSCREEN) - 321) / 2,
        586, 321, nullptr, nullptr, _hInstance, this);
}

void MainForm::show() const {
    ShowWindow(_hwnd, SW_SHOW);
    _sender.run();
    
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT MainForm::windowProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            const CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            MainForm* mainForm = static_cast<MainForm*>(pCreateStruct->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(mainForm));
            mainForm->createControls(hwnd);
            break;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case BTN_APPLY_ID: {
                    MainForm* mainForm = reinterpret_cast<MainForm*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
                    mainForm->updateConfig();
                    MessageBox(hwnd, L"Settings applied successfully!", L"Info", MB_OK | MB_ICONINFORMATION);
                    break;
                }
                case BTN_EXIT_ID:
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                    break;
            }
            break;
        }
        case WM_DESTROY: {
            const MainForm* mainForm = reinterpret_cast<MainForm*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            mainForm->_sender.stop();
            PostQuitMessage(0);
            break;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void MainForm::createControls(const HWND hwnd) {
    const ControlCreator controlCreator(hwnd, _fonts);
    int currentY = 20;
    controlCreator.createLabel(currentY, L"FPS:");
    _hEditFps = controlCreator.createEditControl(currentY, std::to_wstring(_config.fps));

    currentY += ControlCreator::LABEL_HEIGHT + ControlCreator::Y_OFFSET;
    controlCreator.createLabel(currentY, L"Max delay (ms):");
    _hEditMaxDelay = controlCreator.createEditControl(currentY, std::to_wstring(_config.maxDelayMs));

    currentY += ControlCreator::LABEL_HEIGHT + ControlCreator::Y_OFFSET;
    controlCreator.createLabel(currentY, L"Quality (1-100):");
    _hEditQuality = controlCreator.createEditControl(currentY, std::to_wstring(_config.imageConfig.quality));

    currentY += ControlCreator::LABEL_HEIGHT + ControlCreator::Y_OFFSET;
    controlCreator.createLabel(currentY, L"Format (jpg/png):");
    _hComboBoxFormat = controlCreator.createComboBox(currentY, {L"jpg", L"webp"}, static_cast<int>(_config.imageConfig.ext));

    currentY += ControlCreator::CONTROL_HEIGHT + ControlCreator::BUTTON_Y_OFFSET;
    controlCreator.createDefButton(currentY, L"Apply", BTN_APPLY_ID);
    controlCreator.createButton(currentY, L"Exit", BTN_EXIT_ID);
}

void MainForm::updateConfig() {
    static constexpr size_t BUFFER_SIZE = 256;
    wchar_t buffer[BUFFER_SIZE];

    GetWindowText(_hEditFps, buffer, BUFFER_SIZE);
    _config.fps = std::stoi(buffer);

    GetWindowText(_hEditMaxDelay, buffer, BUFFER_SIZE);
    _config.maxDelayMs = std::stoi(buffer);
                
    GetWindowText(_hEditQuality, buffer, BUFFER_SIZE);
    _config.imageConfig.quality = std::stoi(buffer);

    int formatIndex = SendMessage(_hComboBoxFormat, CB_GETCURSEL, 0, 0);
    _config.imageConfig.ext = static_cast<ImageFormat>(formatIndex);
                
    _sender.updateFPSAndMaxDelay(_config.fps, _config.maxDelayMs);
    _sender.updateImageConfig(_config.imageConfig);
}
