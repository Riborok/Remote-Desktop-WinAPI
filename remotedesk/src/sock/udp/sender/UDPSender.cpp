#include "../../../../inc/sock/udp/sender/UDPSender.hpp"

#include "../../../../inc/utils/sock/SockaddrUtils.hpp"
#include "../../../../inc/utils/sock/udp/UDPToolkit.hpp"

UDPSender::UDPSender(const std::string& ip, const u_short port, std::unique_ptr<DataFragmenter> dataFragmenter,
        const DWORD sendBufferSize, const DWORD sendTimeoutMs)
        : _dataFragmenter(std::move(dataFragmenter)) {
    _addr = SockaddrUtils::initializeAddress(ip.c_str(), port);
    _socket.setSendBufferSize(sendBufferSize);
    _socket.setSendTimeout(sendTimeoutMs);
}

void UDPSender::send(const std::vector<byte>& data) {
    const auto dataFragments = _dataFragmenter->createDataFragments(data);
    const auto fragments = createFragments(dataFragments, data.size());
    sendFragments(fragments);
    ++_id;
}

std::vector<std::vector<byte>> UDPSender::createFragments(const std::vector<std::vector<byte>>& dataFragments, const size_t totalSize) const {
    std::vector<std::vector<byte>> fragments(dataFragments.size());
    for (size_t i = 0; i < dataFragments.size(); ++i) {
        fragments[i] = UDPToolkit::createFragment(dataFragments[i], {_id, i, totalSize});
    }
    return fragments;
}

void UDPSender::sendFragments(const std::vector<std::vector<byte>>& fragments) const {
    for (const auto& fragment : fragments) {
        sendFragment(fragment);
    }
}

void UDPSender::sendFragment(const std::vector<byte>& fragment) const {
    _socket.sendToSocket(fragment, _addr);
}
