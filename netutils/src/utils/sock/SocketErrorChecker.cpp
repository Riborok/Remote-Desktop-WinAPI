#include "../../../inc/utils/sock/SocketErrorChecker.hpp"

#include <stdexcept>

void SocketErrorChecker::checkSocket(const SOCKET sock) {
    if (sock == INVALID_SOCKET) {
        throwWSAError("Failed to create socket");
    }
}
void SocketErrorChecker::checkBindError(const int result) {
    if (result == SOCKET_ERROR) {
        throwWSAError("Failed to bind socket");
    }
}
void SocketErrorChecker::checkInetPton(const int result) {
    if (result <= 0) {
        throwWSAError("Invalid IP address format");
    }
}
void SocketErrorChecker::checkListenError(const int result) {
    if (result == SOCKET_ERROR) {
        throwWSAError("Failed to listen on socket");
    }
}
void SocketErrorChecker::checkAcceptError(const SOCKET clientSock) {
    if (clientSock == INVALID_SOCKET) {
        throwWSAError("Failed to accept client connection");
    }
}
void SocketErrorChecker::checkConnectError(const int result) {
    if (result == SOCKET_ERROR) {
        throwWSAError("Failed to connect to server");
    }
}
void SocketErrorChecker::checkSend(const int result) {
    if (result == SOCKET_ERROR) {
        throwWSAError("Failed to send data");
    }
}
void SocketErrorChecker::checkReceive(const int len) {
    if (len == SOCKET_ERROR) {
        throwWSAError("Failed to receive data or connection closed");
    }
}

void SocketErrorChecker::throwWSAError(const std::string& message) {
    const int errorCode = WSAGetLastError();
    throw std::runtime_error(message + ": " + std::to_string(errorCode));
}