#include "../../../inc/utils/sock/SockaddrUtils.hpp"

#include <ws2tcpip.h>

#include "../../../inc/utils/sock/SocketErrorChecker.hpp"

sockaddr_in SockaddrUtils::initializeAddress(const PCSTR ip, const u_short port) {
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    setIpAddress(address, ip);
    return address;
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