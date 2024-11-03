// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32
#include "../../../inc/sock/udp/UDPSender.hpp"

#include "../../../inc/utils/sock/SockaddrUtils.hpp"

UDPSender::UDPSender(const std::string& ip, const u_short port): _socket(SOCK_DGRAM, IPPROTO_UDP) {
    _addr = SockaddrUtils::initializeAddress(ip.c_str(), port);
}

void UDPSender::send(const std::vector<byte>& data) {
    const size_t totalFragments = calculateTotalFragments(data.size());

    for (size_t i = 0; i < totalFragments; ++i) {
        auto payload = createFragmentPayload(data, i);
        sendFragment(payload);
    }
}

size_t UDPSender::calculateTotalFragments(const size_t dataSize) {
    return (dataSize + DATA_SIZE - 1) / DATA_SIZE;
}

std::vector<byte> UDPSender::createFragmentPayload(const std::vector<byte>& data, const size_t fragmentIndex) {
    const size_t offset = fragmentIndex * DATA_SIZE;
    const size_t dataLen = min(DATA_SIZE, data.size() - offset);
    const size_t payloadSize = dataLen + 2*sizeof(size_t);

    std::vector<byte> payload;
    payload.reserve(payloadSize);
    payload.insert(payload.end(), data.begin() + offset, data.begin() + offset + dataLen);
    addSizeTToPacket(payload, fragmentIndex);
    addSizeTToPacket(payload, data.size());

    return payload;
}

void UDPSender::addSizeTToPacket(std::vector<byte>& packet, const size_t value) {
    for (size_t i = 0; i < sizeof(size_t); ++i) {
        packet.push_back(static_cast<byte>((value >> (i * 8)) & 0xFF));
    }
}

void UDPSender::sendFragment(const std::vector<byte>& payload) {
    _socket.sendToSocket(payload, _addr);
}
