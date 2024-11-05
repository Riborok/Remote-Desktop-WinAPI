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

    int sendSocket(const std::vector<byte>& buffer) const;  // NOLINT(modernize-use-nodiscard)
    int sendToSocket(const std::vector<byte>& buffer,
        sockaddr_in& destAddr) const; // NOLINT(modernize-use-nodiscard)

    int recvSocket(std::vector<byte>& buffer) const; // NOLINT(modernize-use-nodiscard)
    int recvFromSocket(std::vector<byte>& buffer,
        sockaddr_in* senderAddr = nullptr, int* senderAddrSize = nullptr) const; // NOLINT(modernize-use-nodiscard)

    void setReceiveTimeout(const DWORD milliseconds) const;
    void setSendTimeout(const DWORD milliseconds) const;

    ~Socket();
    Socket(Socket&&) noexcept;

    Socket& operator=(Socket&&) = delete;
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
private:
    explicit Socket(const SOCKET socket);

    [[nodiscard]] int setTimeout(const DWORD milliseconds, const int option) const;
};
