#pragma once

#include <string>
#include <winsock2.h>

class SockaddrUtils {
public:
    SockaddrUtils() = delete;
    
    static sockaddr_in createAddr(const std::string& ip, const u_short port);
    static sockaddr_in createAddr(const PCSTR ip, const u_short port);
    static std::string getIpAddress(const sockaddr_in& clientAddr);
private:
    static void setIpAddress(sockaddr_in& address, const PCSTR ip);
    static ULONG getAddressFromIp(const PCSTR ip);
};
