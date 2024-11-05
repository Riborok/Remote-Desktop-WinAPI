#pragma once

#include <optional>
#include <vector>
#include <winsock2.h>

#include "Payload.hpp"
#include "../Socket.hpp"
#include "../../utils/MemoryUnits.hpp"

using CryptoPP::byte;

class UDPReceiver {
    Socket _socket;
public:
    explicit UDPReceiver(const u_short port,
        const DWORD receiveTimeoutMs = 1000, const DWORD receiveBufferSize = MemoryUnits::MEGABYTE);
    [[nodiscard]] std::optional<Payload> receivePayload() const;
private:
    int receiveData(std::vector<byte>& data) const;
    static bool isBytesReceivedValid(const int bytesReceived);
    static void populatePayload(Payload& payload, const int bytesReceived);
    static size_t extractTotalSize(const std::vector<byte>& data, const int bytesReceived);
    static size_t extractPacketNumber(const std::vector<byte>& data, const int bytesReceived);
    static size_t extractId(const std::vector<byte>& data, const int bytesReceived);
    static size_t extractSizeTFromPacket(const std::vector<byte>& packet, const size_t startIdx);
};
