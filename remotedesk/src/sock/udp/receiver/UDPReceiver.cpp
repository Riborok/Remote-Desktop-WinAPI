#include "../../../../inc/sock/udp/receiver/UDPReceiver.hpp"

#include "../../../../inc/sock/udp/sender/UDPSender.hpp"
#include "../../../../inc/utils/sock/udp/UDPToolkit.hpp"

UDPReceiver::UDPReceiver(const u_short port, std::unique_ptr<DataReassembler> dataReassembler,
        const DWORD receiveBufferSize, const DWORD receiveTimeoutMs):
            _dataReassembler(std::move(dataReassembler)),
            _fragmentCollector(_dataReassembler->getFragmentDescriptor().getPayloadSize()) {
    _socket.bindSocket(port);
    _socket.setReceiveBufferSize(receiveBufferSize);
    _socket.setReceiveTimeout(receiveTimeoutMs);
}

MaskedData UDPReceiver::receiveMaskedData() {
    std::vector<Fragment> fragments(receiveFragments());
    return _dataReassembler->reassembleData(fragments);
}

std::vector<Fragment> UDPReceiver::receiveFragments() {
    if (_fragmentCollector.collectPreviousFragment()) {
        return _fragmentCollector.extractFragments();
    }

    std::optional<Fragment> optionalFragment;
    do {
        optionalFragment = receiveFragment();
    } while(_fragmentCollector.collectFragment(optionalFragment));
    return _fragmentCollector.extractFragments();
}

std::optional<Fragment> UDPReceiver::receiveFragment() const {
    Fragment fragment;
    const int bytesReceived = receiveData(fragment.payload);
    if (isBytesReceivedValid(bytesReceived)) {
        UDPToolkit::populateFragment(fragment, bytesReceived);
        return std::move(fragment);
    }
    return std::nullopt;
}

int UDPReceiver::receiveData(std::vector<byte>& data) const {
    data.resize(_dataReassembler->getFragmentDescriptor().getSize());
    const int len = _socket.recvFromSocket(data);
    return len;
}

bool UDPReceiver::isBytesReceivedValid(const int bytesReceived) {
    return bytesReceived >= static_cast<int>(UDPToolkit::METADATA_SIZE);
}
