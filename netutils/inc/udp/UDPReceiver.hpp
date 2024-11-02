#pragma once

#include <vector>
#include <winsock2.h>

#include "Payload.hpp"
#include "UDPSender.hpp"
#include "../utils/NetworkUtils.hpp"

using CryptoPP::byte;

class UDPReceiver {
    SOCKET _socket;
public:
    explicit UDPReceiver(const u_short port);
    [[nodiscard]] Payload receive() const;
    ~UDPReceiver();

    UDPReceiver(const UDPReceiver&) = delete;
    UDPReceiver& operator=(const UDPReceiver&) = delete;
    UDPReceiver(UDPReceiver&&) = delete;
    UDPReceiver& operator=(UDPReceiver&&) = delete;
private:
    int receiveData(std::vector<byte>& data) const;
    static size_t extractPacketNumber(const std::vector<byte>& data, const int bytesReceived);
    static size_t extractSizeTFromPacket(const std::vector<byte>& packet, const size_t startIdx);
    static void handleInvalidPacket(Payload& payload);
};
