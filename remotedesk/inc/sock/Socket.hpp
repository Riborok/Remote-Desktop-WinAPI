#pragma once

#include <vector>

#include "../utils/sock/SocketErrorChecker.hpp"

class Socket {
    SOCKET _sock;

    explicit Socket(const SOCKET socket);
public:
    Socket(const int type, const int protocol);

    void bindSocket(const u_short port) const;
    void listenOnSocket(const int backlog) const;
    Socket acceptConnection(sockaddr_in* clientAddr = nullptr, int* clientAddrSize = nullptr) const;
    void connectToServer(const sockaddr_in& serverAddr) const;

    int sendSocket(const std::vector<byte>& buffer) const;
    int sendToSocket(const std::vector<byte>& buffer, const sockaddr_in& destAddr) const;

    int recvSocket(std::vector<byte>& buffer) const;
    int recvFromSocket(std::vector<byte>& buffer,
        sockaddr_in* senderAddr = nullptr, int* senderAddrSize = nullptr) const;

    void setReceiveTimeout(const DWORD milliseconds) const;
    void setSendTimeout(const DWORD milliseconds) const;

    void setSendBufferSize(const DWORD bufferSize) const;
    void setReceiveBufferSize(const DWORD bufferSize) const;
    
    sockaddr_in getPeerAddress() const;

    void shutdownSocket(const int how = SD_BOTH) const;
    
    ~Socket();
    Socket(Socket&&) noexcept;
    Socket& operator=(Socket&&) noexcept;
    
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
private:
    void releaseResources() const noexcept;
    void resetResources() noexcept;
    int setSockOpt(const DWORD value, const int option) const;
};
