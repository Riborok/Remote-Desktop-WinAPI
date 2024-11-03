// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32 CppClangTidyClangDiagnosticSignCompare
#include "../../../inc/sock/udp/UDPReceiver.hpp"

#include "../../../inc/sock/udp/UDPSender.hpp"

UDPReceiver::UDPReceiver(const u_short port): _socket(SOCK_DGRAM, IPPROTO_UDP) {
    _socket.bindSocket(port);
}

Payload UDPReceiver::receivePayload() const {
    Payload payload;

    const int bytesReceived = receiveData(payload.data);
    if (bytesReceived > 2*sizeof(size_t)) {
        payload.totalSize = extractTotalSize(payload.data, bytesReceived);
        payload.packetNumber = extractPacketNumber(payload.data, bytesReceived);
        payload.data.resize(bytesReceived - 2*sizeof(size_t));
    } else {
        handleInvalidPacket(payload);
    }

    return payload;
}

int UDPReceiver::receiveData(std::vector<byte>& data) const {
    data.resize(UDPSender::PAYLOAD_SIZE);
    const int len = _socket.recvFromSocket(data);
    return len;
}

size_t UDPReceiver::extractTotalSize(const std::vector<byte>& data, const int bytesReceived) {
    return extractSizeTFromPacket(data, bytesReceived - sizeof(size_t));
}

size_t UDPReceiver::extractPacketNumber(const std::vector<byte>& data, const int bytesReceived) {
    return extractSizeTFromPacket(data, bytesReceived - 2*sizeof(size_t));
}

size_t UDPReceiver::extractSizeTFromPacket(const std::vector<byte>& packet, const size_t startIdx) {
    size_t value = 0;
    for (size_t i = 0; i < sizeof(size_t); ++i) {
        value |= static_cast<size_t>(packet[startIdx + i]) << (i * 8);
    }
    return value;
}

void UDPReceiver::handleInvalidPacket(Payload& payload) {
    payload.data.clear();
    payload.packetNumber = 0;
    payload.totalSize = 0;
}