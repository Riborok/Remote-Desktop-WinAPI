#include <atomic>
#include <fstream>
#include <thread>
#include <sstream>

#include "sock/Socket.hpp"
#include "utils/sock/tcp/TCPUtils.hpp"
#include "utils/sock/WinSockUtils.hpp"
#include "screen/render/ScreenRender.hpp"
#include "screen/render/ScreenRenderWorker.hpp"
#include "sock/udp/receiver/UDPReceiver.hpp"
#include "sock/udp/data-reassembler/ImgCodecSecureReassembler.hpp"
#include "utils/array/ByteArrayUtils.hpp"
#include "utils/crypto/aes/AESToolkit.hpp"

constexpr u_short DEFAULT_UDP_PORT = 4040;
constexpr u_short DEFAULT_TCP_PORT = 4041;

struct Config {
    u_short udpPort;
    u_short tcpPort;
};

Config parseConfig(const std::string& filename) {
    Config config = {DEFAULT_UDP_PORT, DEFAULT_TCP_PORT};
    std::ifstream file(filename);
    if (!file.is_open()) {
        return config;
    }

    std::unordered_map<std::string, std::string> configMap;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string key, value;
        if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
            configMap[key] = value;
        }
    }
    
    if (configMap.find("UDP_PORT") != configMap.end()) {
        config.udpPort = static_cast<u_short>(std::stoi(configMap["UDP_PORT"]));
    }
    if (configMap.find("TCP_PORT") != configMap.end()) {
        config.tcpPort = static_cast<u_short>(std::stoi(configMap["TCP_PORT"]));
    }

    return config;
}

HWND hwnd;
Config config;

SIZE receiveSize(const Socket& s) {
    std::vector<byte> sizeBuffer(sizeof(int) * 2);
    s.recvSocket(sizeBuffer);
    const int width = ByteArrayUtils::getValue<int>(sizeBuffer, 0);
    const int height = ByteArrayUtils::getValue<int>(sizeBuffer, 1);
    return {width, height};
}

std::atomic<bool> isRunning(true);
std::unique_ptr<ScreenRenderWorker> screenRender = nullptr;

void asyncReceiveData(const Socket& socket) {
    const SIZE remoteSize = receiveSize(socket);
    std::vector<byte> key(AESToolkit::MAX_KEY_LENGTH, 42);
    UDPReceiver receiver(config.udpPort, std::make_unique<ImgCodecSecureReassembler>(remoteSize, key));
    ThreadSafeQueue<std::vector<byte>> queue;
    screenRender = std::make_unique<ScreenRenderWorker>(queue, hwnd, SIZE{5, 5}, remoteSize, 70);
    screenRender->start();
    while (isRunning) {
        queue.enqueue(std::make_unique<std::vector<byte>>(receiver.receive()));
    }
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = windowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"ScreenshotReceiverClass";
    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, L"ScreenshotReceiverClass", L"TCP Screenshot Receiver",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, wc.hInstance, NULL);
    
    WinSockUtils::initializeWinSock();
    config = parseConfig("receiver_cfg.txt");
    
    Socket socket = TCPUtils::acceptSingleConnection(config.tcpPort);
    std::thread receivingThread(asyncReceiveData, std::ref(socket));
    
    ShowWindow(hwnd, SW_SHOW);
    MSG msg = {};
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    screenRender->stop();
    isRunning = false;
    receivingThread.join();
    WinSockUtils::cleanupWinSock();
    return 0;
}

bool resizing = true;

LRESULT CALLBACK windowProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_SIZE: {
            if (wParam != SIZE_MINIMIZED) {
                screenRender->updateAppSize({LOWORD(lParam), HIWORD(lParam)});
            }
            break;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
