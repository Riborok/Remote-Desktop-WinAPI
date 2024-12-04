#pragma once

#include <memory>
#include <vector>
#include <winsock2.h>

#include "../../Socket.hpp"
#include "../../../utils/MemoryUnits.hpp"
#include "../data-fragmenter/DataFragmenter.hpp"

class UDPSender {
    std::unique_ptr<DataFragmenter> _dataFragmenter;
    Socket _socket {SOCK_DGRAM, IPPROTO_UDP};
    sockaddr_in _addr;
public:
    UDPSender(const sockaddr_in& addr, std::unique_ptr<DataFragmenter> dataFragmenter,
              const DWORD sendBufferSize = MemoryUnits::MEGABYTE, const DWORD sendTimeoutMs = 100);
    void send(const std::vector<byte>& data) const;
private:
    void sendFragments(const std::vector<std::vector<byte>>& fragments) const;
    void sendFragment(const std::vector<byte>& fragment) const;
};
