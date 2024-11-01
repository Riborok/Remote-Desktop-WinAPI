#pragma once

#include <winsock2.h>
#include <integer.h>

using CryptoPP::byte;

class NetworkUtils {
public:
    NetworkUtils() = delete;
    
    static SOCKET createSocket(const int type, const int protocol);
    static sockaddr_in initializeAddress(const PCSTR ip, const u_short port);
    static void sendInteger(const SOCKET sock, const CryptoPP::Integer& value);
    static CryptoPP::Integer receiveInteger(const SOCKET sock);
private:
    static void setIpAddress(const PCSTR ip, sockaddr_in& address);
    static ULONG getAddressFromIp(const PCSTR ip);
    
    static void checkSocket(const SOCKET sock);
    static void checkInetPton(const int result);
    static void checkSend(const int result);
    static void checkReceive(const int len);
};
