// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32
#include "../../inc/utils/NetworkUtils.hpp"

#include <ws2tcpip.h>

#include "../../inc/utils/IntegerUtils.hpp"
#include "../../inc/dh/DHHelper.hpp"

SOCKET NetworkUtils::createSocket(const int type, const int protocol) {
    const SOCKET sock = socket(AF_INET, type, protocol);
    checkSocket(sock);
    return sock;
}

void NetworkUtils::checkSocket(const SOCKET sock) {
    if (sock == INVALID_SOCKET) {
        throwWSAError("Failed to create socket");
    }
}

void NetworkUtils::bindSocket(const SOCKET sock, const u_short port) {
    sockaddr_in serverAddr = initializeAddress(nullptr, port);
    const int result = bind(sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    checkBindError(result);
}

void NetworkUtils::checkBindError(const int result) {
    if (result == SOCKET_ERROR) {
        throwWSAError("Failed to bind socket");
    }
}

sockaddr_in NetworkUtils::initializeAddress(const PCSTR ip, const u_short port) {
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    setIpAddress(ip, address);
    return address;
}

void NetworkUtils::setIpAddress(const PCSTR ip, sockaddr_in& address) {
    address.sin_addr.s_addr = ip == nullptr ? INADDR_ANY : getAddressFromIp(ip);
}

ULONG NetworkUtils::getAddressFromIp(const PCSTR ip) {
    in_addr addr;
    const int result = InetPtonA(AF_INET, ip, &addr);
    checkInetPton(result);
    return addr.s_addr;
}

void NetworkUtils::checkInetPton(const int result) {
    if (result <= 0) {
        throwWSAError("Invalid IP address format");
    }
}

void NetworkUtils::sendInteger(const SOCKET sock, const CryptoPP::Integer& value) {
    const std::vector<byte> buffer(IntegerUtils::toVector(value));
    const int result = send(sock, reinterpret_cast<const char*>(buffer.data()), buffer.size(), 0);
    checkSend(result);
}

void NetworkUtils::checkSend(const int result) {
    if (result == SOCKET_ERROR) {
        throwWSAError("Failed to send data");
    }
}

CryptoPP::Integer NetworkUtils::receiveInteger(const SOCKET sock) {
    std::vector<char> buffer(DHHelper::KEY_SIZE);
    const int len = recv(sock, buffer.data(), buffer.size(), 0);
    checkReceive(len);
    return {reinterpret_cast<byte*>(buffer.data()), static_cast<size_t>(len)};
}

void NetworkUtils::checkReceive(const int len) {
    if (len == SOCKET_ERROR) {
        throwWSAError("Failed to receive data or connection closed");
    }
}

void NetworkUtils::checkListenError(const int result) {
    if (result == SOCKET_ERROR) {
        throwWSAError("Failed to listen on socket");
    }
}

void NetworkUtils::throwWSAError(const std::string& message) {
    const int errorCode = WSAGetLastError();
    throw std::runtime_error(message + ": " + std::to_string(errorCode));
}