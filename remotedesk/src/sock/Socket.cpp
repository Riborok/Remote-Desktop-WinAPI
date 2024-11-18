// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32
#include "../../inc/sock/Socket.hpp"

#include "../../inc/utils/sock/SockaddrUtils.hpp"

Socket::Socket(const SOCKET socket): _sock(socket) {
    SocketErrorChecker::checkSocket(_sock);
}

Socket::Socket(const int type, const int protocol): _sock(socket(AF_INET, type, protocol)) {
    SocketErrorChecker::checkSocket(_sock);
}

void Socket::bindSocket(const u_short port) const {
    const sockaddr_in serverAddr = SockaddrUtils::initializeAddress(nullptr, port);
    const int result = bind(_sock, reinterpret_cast<const sockaddr*>(&serverAddr), sizeof(serverAddr));
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

void Socket::connectToServer(const sockaddr_in& serverAddr) const {
    const int result = connect(_sock, reinterpret_cast<const sockaddr*>(&serverAddr), sizeof(serverAddr));
    SocketErrorChecker::checkConnectError(result);
}

int Socket::sendSocket(const std::vector<byte>& buffer) const {
    const int result = send(_sock, reinterpret_cast<const char*>(buffer.data()), buffer.size(), 0);
    return SocketErrorChecker::checkSend(result);
}

int Socket::sendToSocket(const std::vector<byte>& buffer, const sockaddr_in& destAddr) const {
    const int result = sendto(_sock, reinterpret_cast<const char*>(buffer.data()), buffer.size(), 0,
        reinterpret_cast<const sockaddr*>(&destAddr), sizeof(destAddr));
    return SocketErrorChecker::checkSend(result);
}

int Socket::recvSocket(std::vector<byte>& buffer) const {
    const int len = recv(_sock, reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);
    return SocketErrorChecker::checkReceive(len);
}

int Socket::recvFromSocket(std::vector<byte>& buffer, sockaddr_in* senderAddr, int* senderAddrSize) const {
    const int len = recvfrom(_sock, reinterpret_cast<char*>(buffer.data()), buffer.size(), 0,
        reinterpret_cast<sockaddr*>(senderAddr), senderAddrSize);
    return SocketErrorChecker::checkReceive(len);
}

void Socket::setReceiveTimeout(const DWORD milliseconds) const {
    const int result = setSockOpt(milliseconds, SO_RCVTIMEO);
    SocketErrorChecker::checkReceiveTimeoutError(result);
}

void Socket::setSendTimeout(const DWORD milliseconds) const {
    const int result = setSockOpt(milliseconds, SO_SNDTIMEO);
    SocketErrorChecker::checkSendTimeoutError(result);
}

void Socket::setSendBufferSize(const DWORD bufferSize) const {
    const int result = setSockOpt(bufferSize, SO_SNDBUF);
    SocketErrorChecker::checkSetSendBufferError(result);
}

void Socket::setReceiveBufferSize(const DWORD bufferSize) const {
    const int result = setSockOpt(bufferSize, SO_RCVBUF);
    SocketErrorChecker::checkSetReceiveBufferError(result);
}

int Socket::setSockOpt(const DWORD value, const int option) const {
    return setsockopt(_sock, SOL_SOCKET, option, 
        reinterpret_cast<const char*>(&value), sizeof(value));
}

Socket::~Socket() {
    releaseResources();
}

Socket::Socket(Socket&& other) noexcept : _sock(other._sock) {
    other.resetResources();
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        releaseResources();
        _sock = other._sock;
        other.resetResources();
    }
    return *this;
}

void Socket::releaseResources() const noexcept {
    closesocket(_sock);
}

void Socket::resetResources() noexcept {
    _sock = INVALID_SOCKET;
}
