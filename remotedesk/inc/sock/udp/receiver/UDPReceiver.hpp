#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "FragmentCollector.hpp"
#include "FragmentStateTracker.hpp"
#include "../../Socket.hpp"
#include "../../../utils/MemoryUnits.hpp"
#include "../data-reassembler/DataReassembler.hpp"

using CryptoPP::byte;

class UDPReceiver {
    std::unique_ptr<DataReassembler> _dataReassembler;
    Socket _socket {SOCK_DGRAM, IPPROTO_UDP};
    FragmentCollector _fragmentCollector;
public:
    explicit UDPReceiver(const u_short port, std::unique_ptr<DataReassembler> dataReassembler,
                         const DWORD receiveBufferSize = 8*MemoryUnits::MEGABYTE, const DWORD receiveTimeoutMs = 1000);
    MaskedData receiveMaskedData();
private:
    std::vector<Fragment>&& receiveFragments();
    std::optional<Fragment> receiveFragment() const;
    int receiveData(std::vector<byte>& data) const;
    static bool isBytesReceivedValid(const int bytesReceived);
};
