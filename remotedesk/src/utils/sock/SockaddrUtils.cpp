#include "../../../inc/utils/sock/SockaddrUtils.hpp"

#include <ws2tcpip.h>

#include "../../../inc/utils/sock/SocketErrorChecker.hpp"

sockaddr_in SockaddrUtils::createAddr(const std::string& ip, const u_short port) {
    return createAddr(strToIp(ip.c_str()), port);
}

ULONG SockaddrUtils::strToIp(const PCSTR ip) {
    in_addr addr;
    const int result = InetPtonA(AF_INET, ip, &addr);
    SocketErrorChecker::checkInetPton(result);
    return addr.s_addr;
}

std::string SockaddrUtils::ipToStr(const ULONG ip) {
    char ipStr[INET_ADDRSTRLEN];
    InetNtopA(AF_INET, &ip, ipStr, sizeof(ipStr));
    return {ipStr};
}

sockaddr_in SockaddrUtils::createAddrAnyInterface(const u_short port) {
    return createAddr(INADDR_ANY, port);
}

sockaddr_in SockaddrUtils::createAddr(const ULONG ip, const u_short port) {
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = ip;
    return address;
}

ULONG SockaddrUtils::getIpAddress(const sockaddr_in& clientAddr) {
    return clientAddr.sin_addr.s_addr;
}