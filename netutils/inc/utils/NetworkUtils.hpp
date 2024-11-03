#pragma once

#include <winsock2.h>
#include <integer.h>

using CryptoPP::byte;

class NetworkUtils {
public:
    NetworkUtils() = delete;
    
    static SOCKET createSocket(const int type, const int protocol);
    static void bindSocket(const SOCKET sock, const u_short port);
    static sockaddr_in initializeAddress(const PCSTR ip, const u_short port);
    static void listenOnSocket(const SOCKET socket, const int backlog);
    
    static void sendSocket(const SOCKET socket, const std::vector<byte>& buffer);
    static void sendToSocket(const SOCKET socket, const std::vector<byte>& buffer, sockaddr_in& destAddr);
    static int recvSocket(const SOCKET socket, std::vector<byte>& buffer);
    static int recvFromSocket(const SOCKET socket, std::vector<byte>& buffer, sockaddr_in& senderAddr, int& senderAddrSize);

    // TODO: REMOVED THIS
    static void sendInteger(const SOCKET sock, const CryptoPP::Integer& value);
    static CryptoPP::Integer receiveInteger(const SOCKET sock);
private:
    static void setIpAddress(const PCSTR ip, sockaddr_in& address);
    static ULONG getAddressFromIp(const PCSTR ip);
    
    static void checkSocket(const SOCKET sock);
    static void checkBindError(const int result);
    static void checkInetPton(const int result);
    static void checkListenError(const int result);
    static void checkSend(const int result);
    static void checkReceive(const int len);
    static void throwWSAError(const std::string& message);
};
