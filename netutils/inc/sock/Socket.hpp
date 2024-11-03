#pragma once

#include <vector>

#include "../utils/sock/SocketErrorChecker.hpp"

class Socket {
    SOCKET _sock;
public:
    Socket(const int type, const int protocol);

    void bindSocket(const u_short port) const;
    void listenOnSocket(const int backlog) const;
    Socket acceptConnection(sockaddr_in* senderAddr = nullptr, int* senderAddrSize = nullptr) const;
    void connectToServer(sockaddr_in& serverAddr) const;
    
    void sendSocket(const std::vector<byte>& buffer) const;
    void sendToSocket(const std::vector<byte>& buffer, sockaddr_in& destAddr) const;

    int recvSocket(std::vector<byte>& buffer) const;
    int recvFromSocket(std::vector<byte>& buffer, sockaddr_in* senderAddr = nullptr, int* senderAddrSize = nullptr) const;
    
    ~Socket();
    Socket(Socket&&) noexcept;
    
    Socket& operator=(Socket&&) = delete;
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
private:
    explicit Socket(const SOCKET socket): _sock(socket) {
        SocketErrorChecker::checkSocket(_sock);
    }
};
