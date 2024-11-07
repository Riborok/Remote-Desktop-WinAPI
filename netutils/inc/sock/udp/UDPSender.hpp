#pragma once

#include <config_int.h>
#include <vector>
#include <string>
#include <winsock2.h>

#include "../Socket.hpp"
#include "../../utils/MemoryUnits.hpp"

using CryptoPP::byte;

class UDPSender {
    Socket _socket;
    sockaddr_in _addr;
    size_t _id = 1;
public:
    static constexpr size_t PAYLOAD_SIZE = 1500 - 20 - 8;
    static constexpr size_t DATA_SIZE = PAYLOAD_SIZE - 3*sizeof(size_t);

    UDPSender(const std::string& ip, const u_short port,
        const DWORD sendTimeoutMs = 1000, const DWORD sendBufferSize = MemoryUnits::MEGABYTE);
    void send(const std::vector<byte>& data);
private:
    std::vector<byte> createFragmentPayload(const std::vector<byte>& data, const size_t fragmentIndex) const;
    static void addPayloadToPacket(std::vector<byte>& packet, const std::vector<byte>& data, const size_t offset, const size_t dataLen);
    static void addSizeTToPacket(std::vector<byte>& packet, const size_t value, const size_t startIdx);
    void sendFragment(const std::vector<byte>& payload) const;
};
