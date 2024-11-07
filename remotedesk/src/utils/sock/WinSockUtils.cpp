#include "../../../inc/utils/sock/WinSockUtils.hpp"

#include <stdexcept>
#include <string>
#include <ws2tcpip.h>

void WinSockUtils::initializeWinSock() {
    WSADATA wsaData;
    const int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    checkWinSockError(result);
}

void WinSockUtils::checkWinSockError(const int result) {
    if (result != 0) {
        throw std::runtime_error("Failed to initialize WinSock: " + std::to_string(result));
    }
}

void WinSockUtils::cleanupWinSock() {
    WSACleanup();
}
