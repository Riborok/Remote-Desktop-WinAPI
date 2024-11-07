// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32
#include "../../../inc/sock/udp/UDPSender.hpp"

#include "../../../inc/utils/MathUtils.hpp"
#include "../../../inc/utils/TypeLimits.hpp"
#include "../../../inc/utils/sock/SockaddrUtils.hpp"

UDPSender::UDPSender(const std::string& ip, const u_short port,
        const DWORD sendTimeoutMs, const DWORD sendBufferSize)
        : _socket(SOCK_DGRAM, IPPROTO_UDP) {
    _addr = SockaddrUtils::initializeAddress(ip.c_str(), port);
    _socket.setSendTimeout(sendTimeoutMs);
    _socket.setSendBufferSize(sendBufferSize);
}

void UDPSender::send(const std::vector<byte>& data) {
    const size_t totalFragments = MathUtils::ceil(data.size(), DATA_SIZE);

    for (size_t i = 0; i < totalFragments; ++i) {
        std::vector<byte> payload = createFragmentPayload(data, i);
        sendFragment(payload);
    }
    ++_id;
}

std::vector<byte> UDPSender::createFragmentPayload(const std::vector<byte>& data, const size_t packetNumber) const {
    const size_t offset = packetNumber * DATA_SIZE;
    const size_t dataLen = min(DATA_SIZE, data.size() - offset);
    const size_t payloadSize = dataLen + 3*sizeof(size_t);

    std::vector<byte> payload(payloadSize);
    addPayloadToPacket(payload, data, offset, dataLen);
    addId(payload, _id, dataLen);
    addPacketNumber(payload, packetNumber, dataLen);
    addTotalSize(payload, data.size(), dataLen);
    return payload;
}

void UDPSender::addPayloadToPacket(std::vector<byte>& packet, const std::vector<byte>& data,
        const size_t offset, const size_t dataLen) {
    std::memcpy(packet.data(), &data[offset], dataLen);
}

void UDPSender::addId(std::vector<byte>& payload, const size_t id, const size_t dataLen) {
    addSizeTToPacket(payload, id, dataLen);
}

void UDPSender::addPacketNumber(std::vector<byte>& payload, const size_t packetNumber, const size_t dataLen) {
    addSizeTToPacket(payload, packetNumber, dataLen + sizeof(size_t));
}

void UDPSender::addTotalSize(std::vector<byte>& payload, const size_t totalSize, const size_t dataLen) {
    addSizeTToPacket(payload, totalSize, dataLen + 2*sizeof(size_t));
}

void UDPSender::addSizeTToPacket(std::vector<byte>& packet, const size_t value, const size_t startIdx) {
    std::memcpy(&packet[startIdx], &value, sizeof(size_t));
}

void UDPSender::sendFragment(const std::vector<byte>& payload) const {
    _socket.sendToSocket(payload, _addr);
}
