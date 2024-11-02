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

    UDPSender(const std::string& ip, const u_short port);
    void send(const std::vector<byte>& data);
    ~UDPSender();

    UDPSender(const UDPSender&) = delete;
    UDPSender& operator=(const UDPSender&) = delete;
    UDPSender(UDPSender&&) = delete;
    UDPSender& operator=(UDPSender&&) = delete;

private:
    static size_t calculateTotalFragments(const size_t dataSize);
    static std::vector<byte> createFragmentPayload(const std::vector<byte>& data, const size_t fragmentIndex);
    static void addSizeTToPacket(std::vector<byte>& packet, const size_t value);
    void sendFragment(const std::vector<byte>& payload);
};
