#pragma once

#include <config_int.h>
#include <vector>
#include <string>
#include <winsock2.h>

#include "../Socket.hpp"

using CryptoPP::byte;

class UDPSender {
    Socket _socket;
    sockaddr_in _addr;
public:
    static constexpr size_t PAYLOAD_SIZE = 1500 - 20 - 8;
    static constexpr size_t DATA_SIZE = PAYLOAD_SIZE - sizeof(size_t);

    UDPSender(const std::string& ip, const u_short port);
    void send(const std::vector<byte>& data);
private:
    static size_t calculateTotalFragments(const size_t dataSize);
    static std::vector<byte> createFragmentPayload(const std::vector<byte>& data, const size_t fragmentIndex);
    static void addSizeTToPacket(std::vector<byte>& packet, const size_t value);
    void sendFragment(const std::vector<byte>& payload);
};
