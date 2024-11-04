// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32 CppClangTidyClangDiagnosticSignCompare
#include "../../../inc/sock/udp/UDPReceiver.hpp"

#include <stdexcept>

#include "../../../inc/sock/udp/UDPSender.hpp"

UDPReceiver::UDPReceiver(const u_short port): _socket(SOCK_DGRAM, IPPROTO_UDP) {
    _socket.bindSocket(port);
}

Payload UDPReceiver::receivePayload() const {
    Payload payload;
    const int bytesReceived = receiveData(payload.data);
    checkBytesReceived(bytesReceived);
    populatePayload(payload, bytesReceived);
    return payload;
}

int UDPReceiver::receiveData(std::vector<byte>& data) const {
    data.resize(UDPSender::PAYLOAD_SIZE);
    const int len = _socket.recvFromSocket(data);
    return len;
}

void UDPReceiver::checkBytesReceived(const size_t bytesReceived) {
    if (bytesReceived < 3*sizeof(size_t)) {
        throw std::runtime_error("Received bytes less than 3*sizeof(size_t) for UDP");
    }
}

void UDPReceiver::populatePayload(Payload& payload, const int bytesReceived) {
    payload.totalDataSize = extractTotalSize(payload.data, bytesReceived);
    payload.packetNumber = extractPacketNumber(payload.data, bytesReceived);
    payload.id = extractId(payload.data, bytesReceived);
    payload.data.resize(bytesReceived - 3*sizeof(size_t));
}

size_t UDPReceiver::extractTotalSize(const std::vector<byte>& data, const int bytesReceived) {
    return extractSizeTFromPacket(data, bytesReceived - sizeof(size_t));
}

size_t UDPReceiver::extractPacketNumber(const std::vector<byte>& data, const int bytesReceived) {
    return extractSizeTFromPacket(data, bytesReceived - 2*sizeof(size_t));
}

size_t UDPReceiver::extractId(const std::vector<byte>& data, const int bytesReceived) {
    return extractSizeTFromPacket(data, bytesReceived - 3*sizeof(size_t));
}

size_t UDPReceiver::extractSizeTFromPacket(const std::vector<byte>& packet, const size_t startIdx) {
    size_t value = 0;
    for (size_t i = 0; i < sizeof(size_t); ++i) {
        value |= static_cast<size_t>(packet[startIdx + i]) << (i * 8);
    }
    return value;
}
