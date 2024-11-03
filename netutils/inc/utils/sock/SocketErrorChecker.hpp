#pragma once

#include <string>
#include <winsock2.h>

class SocketErrorChecker {
public:
    SocketErrorChecker() = delete;

    static void checkSocket(const SOCKET sock);
    static void checkBindError(const int result);
    static void checkInetPton(const int result);
    static void checkListenError(const int result);
    static void checkAcceptError(const SOCKET clientSock);
    static void checkConnectError(const int result);
    static void checkSend(const int result);
    static void checkReceive(const int len);
private:
    static void throwWSAError(const std::string& message);
};
