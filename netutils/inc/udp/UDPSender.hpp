#pragma once

#include <vector>
#include <string>
#include <winsock2.h>

#include "../utils/NetworkUtils.hpp"

using CryptoPP::byte;

class UDPSender {
    SOCKET _socket;
    sockaddr_in _addr;
public:
    static constexpr size_t PAYLOAD_SIZE = 1500 - 20 - 8;
    static constexpr size_t DATA_SIZE = PAYLOAD_SIZE - sizeof(size_t);

    UDPSender(const std::string& ip, const u_short port) {
        _socket = NetworkUtils::createSocket(SOCK_DGRAM, IPPROTO_UDP);
        _addr = NetworkUtils::initializeAddress(ip.c_str(), port);
    }

    void send(const std::vector<byte>& data) {
        const size_t totalFragments = calculateTotalFragments(data.size());

        for (size_t i = 0; i < totalFragments; ++i) {
            auto payload = createFragmentPayload(data, i);
            sendFragment(payload);
        }
    }

    ~UDPSender() {
        closesocket(_socket);
    }

    UDPSender(const UDPSender&) = delete;
    UDPSender& operator=(const UDPSender&) = delete;
    UDPSender(UDPSender&&) = delete;
    UDPSender& operator=(UDPSender&&) = delete;

private:
    static size_t calculateTotalFragments(const size_t dataSize) {
        return (dataSize + DATA_SIZE - 1) / DATA_SIZE;
    }

    static std::vector<byte> createFragmentPayload(const std::vector<byte>& data, const size_t fragmentIndex) {
        const size_t offset = fragmentIndex * DATA_SIZE;
        const size_t dataLen = min(DATA_SIZE, data.size() - offset);
        const size_t payloadSize = dataLen + sizeof(size_t);

        std::vector<byte> payload;
        payload.reserve(payloadSize);
        payload.insert(payload.end(), data.begin() + offset, data.begin() + offset + dataLen);
        addSizeTToPacket(payload, fragmentIndex);

        return payload;
    }

    static void addSizeTToPacket(std::vector<byte>& packet, const size_t value) {
        for (size_t i = 0; i < sizeof(size_t); ++i) {
            packet.push_back(static_cast<byte>((value >> (i * 8)) & 0xFF));
        }
    }
    
    void sendFragment(const std::vector<byte>& payload) {
        const int result = sendto(_socket, reinterpret_cast<const char*>(payload.data()), payload.size(), 0,
            reinterpret_cast<sockaddr*>(&_addr), sizeof(_addr));
    }
};
