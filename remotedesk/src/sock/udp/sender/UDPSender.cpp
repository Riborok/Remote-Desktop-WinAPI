#include "../../../../inc/sock/udp/sender/UDPSender.hpp"

#include "../../../../inc/utils/sock/SockaddrUtils.hpp"

UDPSender::UDPSender(const sockaddr_in& addr, std::unique_ptr<DataFragmenter> dataFragmenter,
        const DWORD sendBufferSize, const DWORD sendTimeoutMs):
        _dataFragmenter(std::move(dataFragmenter)), _addr(addr) {
    _socket.setSendBufferSize(sendBufferSize);
    _socket.setSendTimeout(sendTimeoutMs);
}

void UDPSender::send(const std::vector<byte>& data) const {
    const std::vector<std::vector<byte>> fragments = _dataFragmenter->createFragments(data);
    sendFragments(fragments);
}

void UDPSender::sendFragments(const std::vector<std::vector<byte>>& fragments) const {
    for (const auto& fragment : fragments) {
        sendFragment(fragment);
    }
}

void UDPSender::sendFragment(const std::vector<byte>& fragment) const {
    _socket.sendToSocket(fragment, _addr);
}
