#pragma once

#include <string>
#include <winsock2.h>

class SockaddrUtils {
public:
    SockaddrUtils() = delete;
    
    static sockaddr_in createAddr(const std::string& ip, const u_short port);
    static ULONG strToIp(const PCSTR ip);
    static std::string ipToStr(const ULONG ip);
    static sockaddr_in createAddrAnyInterface(const u_short port);
    static sockaddr_in createAddr(const ULONG ip, const u_short port);
    static ULONG getIpAddress(const sockaddr_in& clientAddr);
};
