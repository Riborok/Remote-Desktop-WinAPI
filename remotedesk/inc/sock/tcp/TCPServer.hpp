﻿#pragma once

#include "TCPConnection.hpp"

class TCPServer {
    Socket _listenSocket;
public:
    explicit TCPServer(const u_short port);

    TCPConnection acceptConnection(sockaddr_in* clientAddr = nullptr, int* clientAddrSize = nullptr) const;

    ~TCPServer() = default;
    TCPServer(TCPServer&&) noexcept = default;
    TCPServer& operator=(TCPServer&&) noexcept = default;

    TCPServer(const TCPServer&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;
};
