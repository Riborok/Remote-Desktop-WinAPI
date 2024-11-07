#pragma once

#include <string>
#include <winsock2.h>

class SocketErrorChecker {
public:
    SocketErrorChecker() = delete;
    
    static constexpr int SOCKET_TIMEOUT = -2;

    static void checkSocket(const SOCKET sock);
    static void checkBindError(const int result);
    static void checkInetPton(const int result);
    static void checkListenError(const int result);
    static void checkAcceptError(const SOCKET clientSock);
    static void checkConnectError(const int result);
    static int checkSend(const int result);
    static int checkReceive(const int len);
    static void checkReceiveTimeoutError(const int result);
    static void checkSendTimeoutError(const int result);
    static void checkSetSendBufferError(const int result);
    static void checkSetReceiveBufferError(const int result);
private:
    static void checkSetSockOptError(const int result, const std::string& option);
    [[noreturn]] static void throwWSAError(const std::string& message);
    [[noreturn]] static void throwWSAError(const std::string& message, const int errorCode);
};
