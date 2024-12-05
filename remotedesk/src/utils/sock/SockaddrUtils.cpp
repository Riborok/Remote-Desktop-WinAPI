#include "../../../inc/utils/sock/SockaddrUtils.hpp"

#include <ws2tcpip.h>

#include "../../../inc/utils/sock/SocketErrorChecker.hpp"

sockaddr_in SockaddrUtils::createAddr(const std::string& ip, const u_short port) {
    return createAddr(ip.c_str(), port);
}

sockaddr_in SockaddrUtils::createAddr(const PCSTR ip, const u_short port) {
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    setIpAddress(address, ip);
    return address;
}

std::string SockaddrUtils::getIpAddress(const sockaddr_in& clientAddr) {
    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, ipStr, sizeof(ipStr));
    return {ipStr};
}

void SockaddrUtils::setIpAddress(sockaddr_in& address, const PCSTR ip) {
    address.sin_addr.s_addr = ip == nullptr ? INADDR_ANY : getAddressFromIp(ip);
}

ULONG SockaddrUtils::getAddressFromIp(const PCSTR ip) {
    in_addr addr;
    const int result = InetPtonA(AF_INET, ip, &addr);
    SocketErrorChecker::checkInetPton(result);
    return addr.s_addr;
}