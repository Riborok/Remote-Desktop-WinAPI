// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32 CppClangTidyClangDiagnosticSignCompare
#include "../../../inc/sock/udp/UDPReceiver.hpp"

#include "../../../inc/sock/udp/UDPSender.hpp"

UDPReceiver::UDPReceiver(const u_short port,
        const DWORD receiveBufferSize, const DWORD receiveTimeoutMs)
        : _socket(SOCK_DGRAM, IPPROTO_UDP) {
    _socket.bindSocket(port);
    _socket.setReceiveBufferSize(receiveBufferSize);
    _socket.setReceiveTimeout(receiveTimeoutMs);
}

MaskedData UDPReceiver::receiveMaskedData() {
    MaskedDataContext mdc;

    if (processPreviousPayload(mdc)) {
        return mdc.extractMaskedData();
    }

    std::optional<Payload> optionalPayload;
    do {
        optionalPayload = receivePayload();
    } while(hasPayloadAndInProgress(optionalPayload, mdc) || mdc.isEmpty());
    return mdc.extractMaskedData();
}

bool UDPReceiver::processPreviousPayload(MaskedDataContext& mdc) {
    if (_prevPayload) {
        _prevId = _prevPayload->id;
        mdc.initializeFromPayload(*_prevPayload);
        _prevPayload.reset();
        return mdc.isAllDataReceived();
    }
    return false;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
bool UDPReceiver::hasPayloadAndInProgress(std::optional<Payload>& optionalPayload, MaskedDataContext& mdc) {
    return optionalPayload.has_value() && !processPayload(*optionalPayload, mdc);
}

bool UDPReceiver::processPayload(Payload& payload, MaskedDataContext& mdc) {
    if (!mdc.isInitialized()) {
        return processUninitializedContext(payload, mdc);
    }
    if (mdc.isOldPayload(payload)) {
        return processOldPayload(mdc);
    }
    if (mdc.isNewPayload(payload)) {
        return processNewPayload(payload);
    }
    return processCurrentPayload(payload, mdc);
}

bool UDPReceiver::processUninitializedContext(const Payload& payload, MaskedDataContext& mdc) {
    if (_prevId < payload.id) {
        _prevId = payload.id;
        mdc.initializeFromPayload(payload);
        return mdc.isAllDataReceived();
    }
    return false;
}

bool UDPReceiver::processOldPayload(const MaskedDataContext& mdc) {
    if (mdc.hasExceededOldPayloadCountInRow()) {
        return true;
    }
    return false;
}

bool UDPReceiver::processNewPayload(Payload& payload) {
    _prevPayload = std::move(payload);
    return true;
}

bool UDPReceiver::processCurrentPayload(const Payload& payload, MaskedDataContext& mdc) {
    mdc.insertPayload(payload);
    return mdc.isAllDataReceived();
}

std::optional<Payload> UDPReceiver::receivePayload() const {
    Payload payload;
    const int bytesReceived = receiveData(payload.data);
    if (isBytesReceivedValid(bytesReceived)) {
        populatePayload(payload, bytesReceived);
        return std::move(payload);
    }
    return std::nullopt;
}

int UDPReceiver::receiveData(std::vector<byte>& data) const {
    data.resize(UDPSender::PAYLOAD_SIZE);
    const int len = _socket.recvFromSocket(data);
    return len;
}

bool UDPReceiver::isBytesReceivedValid(const int bytesReceived) {
    return bytesReceived >= static_cast<int>(3*sizeof(size_t));
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
    size_t value;
    std::memcpy(&value, &packet[startIdx], sizeof(size_t));
    return value;
}
