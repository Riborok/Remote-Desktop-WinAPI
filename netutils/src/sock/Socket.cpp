// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32
#include "../../inc/sock/Socket.hpp"

#include "../../inc/utils/sock/SockaddrUtils.hpp"

Socket::Socket(const int type, const int protocol): _sock(socket(AF_INET, type, protocol)) {
    SocketErrorChecker::checkSocket(_sock);
}

void Socket::bindSocket(const u_short port) const {
    sockaddr_in serverAddr = SockaddrUtils::initializeAddress(nullptr, port);
    const int result = bind(_sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    SocketErrorChecker::checkBindError(result);
}

void Socket::listenOnSocket(const int backlog) const {
    const int result = listen(_sock, backlog);
    SocketErrorChecker::checkListenError(result);
}

Socket Socket::acceptConnection(sockaddr_in* senderAddr, int* senderAddrSize) const {
    const SOCKET clientSock = accept(_sock, reinterpret_cast<sockaddr*>(senderAddr), senderAddrSize);
    SocketErrorChecker::checkAcceptError(clientSock);
    return Socket(clientSock);
}

void Socket::connectToServer(sockaddr_in& serverAddr) const {
    const int result = connect(_sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    SocketErrorChecker::checkConnectError(result);
}

void Socket::sendSocket(const std::vector<byte>& buffer) const {
    const int result = send(_sock, reinterpret_cast<const char*>(buffer.data()), buffer.size(), 0);
    SocketErrorChecker::checkSend(result);
}

void Socket::sendToSocket(const std::vector<byte>& buffer, sockaddr_in& destAddr) const {
    const int result = sendto(_sock, reinterpret_cast<const char*>(buffer.data()), buffer.size(), 0,
        reinterpret_cast<sockaddr*>(&destAddr), sizeof(destAddr));
    SocketErrorChecker::checkSend(result);
}

int Socket::recvSocket(std::vector<byte>& buffer) const {
    const int len = recv(_sock, reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);
    SocketErrorChecker::checkReceive(len);
    return len;
}

int Socket::recvFromSocket(std::vector<byte>& buffer, sockaddr_in* senderAddr, int* senderAddrSize) const {
    const int len = recvfrom(_sock, reinterpret_cast<char*>(buffer.data()), buffer.size(), 0,
        reinterpret_cast<sockaddr*>(senderAddr), senderAddrSize);
    SocketErrorChecker::checkReceive(len);
    return len;
}

Socket::~Socket() {
    closesocket(_sock);
}

Socket::Socket(Socket&& other) noexcept : _sock(other._sock) {
    other._sock = INVALID_SOCKET;
}
