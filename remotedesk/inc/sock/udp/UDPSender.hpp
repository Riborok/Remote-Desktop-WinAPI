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
        const DWORD sendBufferSize = MemoryUnits::MEGABYTE, const DWORD sendTimeoutMs = 1000);
    void send(const std::vector<byte>& data);
private:
    std::vector<byte> createFragmentPayload(const std::vector<byte>& data, const size_t fragmentNumber) const;
    static void addData(std::vector<byte>& payload, const std::vector<byte>& data, const size_t offset, const size_t dataLen);
    static void addId(std::vector<byte>& payload, const size_t id, const size_t dataLen);
    static void addFragmentNumber(std::vector<byte>& payload, const size_t fragmentNumber, const size_t dataLen);
    static void addTotalSize(std::vector<byte>& payload, const size_t totalSize, const size_t dataLen);
    static void addSizeTToPayload(std::vector<byte>& payload, const size_t value, const size_t startIdx);
    void sendFragment(const std::vector<byte>& payload) const;
};
