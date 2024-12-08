#include "remote-desk/receiver/Receiver.hpp"
#include "utils/sock/WinSockUtils.hpp"
#include "utils/sock/SockaddrUtils.hpp"

const std::wstring WINDOWS_CLASS_NAME = L"ScreenshotReceiverClass";

static std::unique_ptr<Receiver> receiver;

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(const HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WinSockUtils::initializeWinSock();
    
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = windowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WINDOWS_CLASS_NAME.c_str();
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(0, WINDOWS_CLASS_NAME.c_str(), L"TCP Screenshot Receiver",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        nullptr, nullptr, hInstance, nullptr);
    
    receiver = std::make_unique<Receiver>(hwnd, SockaddrUtils::createAddr("192.168.31.2", 8080), 8081, 100, 1000);
    receiver->run();

    ShowWindow(hwnd, SW_SHOW);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    receiver->stop();
    WinSockUtils::cleanupWinSock();
    return 0;
}

LRESULT CALLBACK windowProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_SIZE: {
            if (wParam != SIZE_MINIMIZED) {
                receiver->updateAppSize({LOWORD(lParam), HIWORD(lParam)});
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
            receiver->sendEvent(uMsg, wParam, lParam);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
