#pragma once

#include <winsock2.h>

class SockaddrUtils {
public:
    static sockaddr_in initializeAddress(const PCSTR ip, const u_short port);
private:
    static void setIpAddress(sockaddr_in& address, const PCSTR ip);
    static ULONG getAddressFromIp(const PCSTR ip);
};
