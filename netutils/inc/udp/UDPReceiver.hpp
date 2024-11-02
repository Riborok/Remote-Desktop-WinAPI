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
    explicit UDPReceiver(const u_short port) {
        _socket = NetworkUtils::createSocket(SOCK_DGRAM, IPPROTO_UDP);
        NetworkUtils::bindSocket(_socket, port);
    }

    [[nodiscard]] Payload receive() const {
        Payload payload;

        const int bytesReceived = receiveData(payload.data);
        if (bytesReceived > sizeof(size_t)) {
            payload.packetNumber = extractPacketNumber(payload.data, bytesReceived);
            payload.data.resize(bytesReceived - sizeof(size_t));
        } else {
            handleInvalidPacket(payload);
        }

        return payload;
    }

    ~UDPReceiver() {
        closesocket(_socket);
    }

    UDPReceiver(const UDPReceiver&) = delete;
    UDPReceiver& operator=(const UDPReceiver&) = delete;
    UDPReceiver(UDPReceiver&&) = delete;
    UDPReceiver& operator=(UDPReceiver&&) = delete;
private:
    int receiveData(std::vector<byte>& data) const {
        data.resize(UDPSender::PAYLOAD_SIZE);
        sockaddr_in senderAddr;
        int senderAddrSize = sizeof(senderAddr);
        
        const int len = recvfrom(_socket, reinterpret_cast<char*>(data.data()), data.size(), 0,
            reinterpret_cast<sockaddr*>(&senderAddr), &senderAddrSize);
        return len;
    }

    static size_t extractPacketNumber(const std::vector<byte>& data, const int bytesReceived) {
        return extractSizeTFromPacket(data, bytesReceived - sizeof(size_t));
    }

    static size_t extractSizeTFromPacket(const std::vector<byte>& packet, const size_t startIdx) {
        size_t value = 0;
        for (size_t i = 0; i < sizeof(size_t); ++i) {
            value |= static_cast<size_t>(packet[startIdx + i]) << (i * 8);
        }
        return value;
    }

    static void handleInvalidPacket(Payload& payload) {
        payload.packetNumber = 0;
        payload.data.clear();
    }
};
