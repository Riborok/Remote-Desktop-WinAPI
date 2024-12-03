#pragma once

#include "../Socket.hpp"

class TCPConnection {
    friend class TCPServer;
    
    Socket _socket;

    explicit TCPConnection(Socket&& socket);
public:
    explicit TCPConnection(const std::string& ip, const u_short port);
    int sendData(const std::vector<byte>& buffer) const;
    int receiveData(std::vector<byte>& buffer) const;
    void setReceiveTimeout(const DWORD milliseconds) const;
    void setSendTimeout(const DWORD milliseconds) const;

    ~TCPConnection() = default;
    TCPConnection(TCPConnection&&) noexcept = default;
    TCPConnection& operator=(TCPConnection&&) noexcept = default;

    TCPConnection(const TCPConnection&) = delete;
    TCPConnection& operator=(const TCPConnection&) = delete;
};
