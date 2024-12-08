#include "remote-desk/sender/Sender.hpp"
#include "utils/sock/WinSockUtils.hpp"

const std::wstring WINDOWS_CLASS_NAME = L"ScreenshotSenderClass";

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(const HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WinSockUtils::initializeWinSock();

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = windowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WINDOWS_CLASS_NAME.c_str();
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(0, WINDOWS_CLASS_NAME.c_str(), L"TCP Screenshot Sender",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        nullptr, nullptr, hInstance, nullptr);
    
    Sender sender(8080, {ImageFormat::jpg, 60}, 30, 1000);
    sender.run();
    
    ShowWindow(hwnd, SW_SHOW);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    sender.stop();
    WinSockUtils::cleanupWinSock();
    return 0;
}

LRESULT CALLBACK windowProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}