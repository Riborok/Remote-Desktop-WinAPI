#pragma once

#include <optional>
#include <vector>

#include "MaskedDataContext.hpp"

using CryptoPP::byte;

class UDPReceiver {
    Socket _socket;
    std::optional<Payload> _prevPayload;
    size_t _prevId = 0;
public:
    explicit UDPReceiver(const u_short port,
        const DWORD receiveTimeoutMs = 1000, const DWORD receiveBufferSize = MemoryUnits::MEGABYTE);
    [[nodiscard]] MaskedData receiveMaskedData();
    [[nodiscard]] std::optional<Payload> receivePayload() const;
private:
    bool processPreviousPayload(MaskedDataContext& mdc);
    bool hasPayloadAndInProgress(std::optional<Payload>& optionalPayload, MaskedDataContext& mdc);
    bool processPayload(Payload& payload, MaskedDataContext& mdc);
    bool processUninitializedContext(const Payload& payload, MaskedDataContext& mdc);
    static bool processOldPayload(const MaskedDataContext& mdc);
    bool processNewPayload(Payload& payload);
    static bool processCurrentPayload(const Payload& payload, MaskedDataContext& mdc);
    
    int receiveData(std::vector<byte>& data) const;
    static bool isBytesReceivedValid(const int bytesReceived);
    static void populatePayload(Payload& payload, const int bytesReceived);
    
    static size_t extractTotalSize(const std::vector<byte>& data, const int bytesReceived);
    static size_t extractPacketNumber(const std::vector<byte>& data, const int bytesReceived);
    static size_t extractId(const std::vector<byte>& data, const int bytesReceived);
    static size_t extractSizeTFromPacket(const std::vector<byte>& packet, const size_t startIdx);
};
