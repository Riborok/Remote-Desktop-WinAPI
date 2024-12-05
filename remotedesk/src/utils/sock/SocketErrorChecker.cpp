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

int SocketErrorChecker::checkSend(const int result) {
    if (result == SOCKET_ERROR) {
        const int errorCode = WSAGetLastError();
        if (errorCode == WSAETIMEDOUT) {
            return SOCKET_TIMEOUT;
        }
        throwWSAError("Failed to send data", errorCode);
    }
    return result;
}

int SocketErrorChecker::checkReceive(const int len) {
    if (len == SOCKET_ERROR) {
        const int errorCode = WSAGetLastError();
        if (errorCode == WSAETIMEDOUT) {
            return SOCKET_TIMEOUT;
        }
        throwWSAError("Failed to receive data or connection closed", errorCode);
    }
    return len;
}

void SocketErrorChecker::checkReceiveTimeoutError(const int result) {
    checkSetSockOptError(result, "receive timeout");
}

void SocketErrorChecker::checkSendTimeoutError(const int result) {
    checkSetSockOptError(result, "send timeout");
}

void SocketErrorChecker::checkSetSendBufferError(const int result) {
    checkSetSockOptError(result, "send buffer size");
}

void SocketErrorChecker::checkSetReceiveBufferError(const int result) {
    checkSetSockOptError(result, "receive buffer size");
}

void SocketErrorChecker::checkSetSockOptError(const int result, const std::string& option) {
    if (result == SOCKET_ERROR) {
        throwWSAError("Failed to set socket option: " + option);
    }
}

void SocketErrorChecker::checkGetPeerAddress(const int result) {
    if (result == SOCKET_ERROR) {
        throwWSAError("Failed to get peer address");
    }
}

void SocketErrorChecker::throwWSAError(const std::string& message) {
    const int errorCode = WSAGetLastError();
    throwWSAError(message, errorCode);
}

void SocketErrorChecker::throwWSAError(const std::string& message, const int errorCode) {
    throw std::runtime_error(message + ": " + std::to_string(errorCode));
}