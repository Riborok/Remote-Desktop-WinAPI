// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32
#include "../../../inc/sock/udp/UDPSender.hpp"

#include "../../../inc/utils/MathUtils.hpp"
#include "../../../inc/utils/TypeLimits.hpp"
#include "../../../inc/utils/sock/SockaddrUtils.hpp"

UDPSender::UDPSender(const std::string& ip, const u_short port,
        const DWORD sendBufferSize, const DWORD sendTimeoutMs)
        : _socket(SOCK_DGRAM, IPPROTO_UDP) {
    _addr = SockaddrUtils::initializeAddress(ip.c_str(), port);
    _socket.setSendBufferSize(sendBufferSize);
    _socket.setSendTimeout(sendTimeoutMs);
}

void UDPSender::send(const std::vector<byte>& data) {
    const size_t totalFragments = MathUtils::ceil(data.size(), DATA_SIZE);

    for (size_t i = 0; i < totalFragments; ++i) {
        std::vector<byte> payload = createFragmentPayload(data, i);
        sendFragment(payload);
    }
    ++_id;
}

std::vector<byte> UDPSender::createFragmentPayload(const std::vector<byte>& data, const size_t fragmentNumber) const {
    const size_t offset = fragmentNumber * DATA_SIZE;
    const size_t dataLen = min(DATA_SIZE, data.size() - offset);
    const size_t payloadSize = dataLen + 3*sizeof(size_t);

    std::vector<byte> payload(payloadSize);
    addData(payload, data, offset, dataLen);
    addId(payload, _id, dataLen);
    addFragmentNumber(payload, fragmentNumber, dataLen);
    addTotalSize(payload, data.size(), dataLen);
    return payload;
}

void UDPSender::addData(std::vector<byte>& payload, const std::vector<byte>& data,
        const size_t offset, const size_t dataLen) {
    std::memcpy(payload.data(), &data[offset], dataLen);
}

void UDPSender::addId(std::vector<byte>& payload, const size_t id, const size_t dataLen) {
    addSizeTToPayload(payload, id, dataLen);
}

void UDPSender::addFragmentNumber(std::vector<byte>& payload, const size_t fragmentNumber, const size_t dataLen) {
    addSizeTToPayload(payload, fragmentNumber, dataLen + sizeof(size_t));
}

void UDPSender::addTotalSize(std::vector<byte>& payload, const size_t totalSize, const size_t dataLen) {
    addSizeTToPayload(payload, totalSize, dataLen + 2*sizeof(size_t));
}

void UDPSender::addSizeTToPayload(std::vector<byte>& payload, const size_t value, const size_t startIdx) {
    std::memcpy(&payload[startIdx], &value, sizeof(size_t));
}

void UDPSender::sendFragment(const std::vector<byte>& payload) const {
    _socket.sendToSocket(payload, _addr);
}
