#pragma once

#include "../sock/tcp/TCPConnection.hpp"

class ParameterCommunicator {
    TCPConnection& _connection;
public:
    explicit ParameterCommunicator(TCPConnection& connection);

    void sendSize(const SIZE& size) const;
    SIZE receiveSize() const;
    void sendUDPPort(const u_short udpPort) const;
    u_short receiveUDPPort() const;

    ~ParameterCommunicator() = default;
    ParameterCommunicator(ParameterCommunicator&&) = delete;
    ParameterCommunicator& operator=(ParameterCommunicator&&) = delete;
    ParameterCommunicator(const ParameterCommunicator&) = delete;
    ParameterCommunicator& operator=(const ParameterCommunicator&) = delete;
};
