#pragma once

#include <vector>
#include <winsock2.h>

#include "Payload.hpp"
#include "../Socket.hpp"

using CryptoPP::byte;

class UDPReceiver {
    Socket _socket;
public:
    explicit UDPReceiver(const u_short port);
    [[nodiscard]] Payload receive() const;
private:
    int receiveData(std::vector<byte>& data) const;
    static size_t extractPacketNumber(const std::vector<byte>& data, const int bytesReceived);
    static size_t extractSizeTFromPacket(const std::vector<byte>& packet, const size_t startIdx);
    static void handleInvalidPacket(Payload& payload);
};
