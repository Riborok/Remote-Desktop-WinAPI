#include "../../../../inc/sock/udp/receiver/MaskedData.hpp"

#include "../../../../inc/sock/udp/sender/UDPSender.hpp"
#include "../../../../inc/utils/TypeLimits.hpp"
#include "../../../../inc/utils/sock/udp/UDPToolkit.hpp"

void MaskedData::resize(const size_t size) {
    _data.resize(size, 0);
    _mask.resize(size, TypeLimits::MAX_BYTE);
}

void MaskedData::insert(const Fragment& fragment) {
    const size_t offset = fragment.fragmentNumber * UDPToolkit::DATA_SIZE;
    std::memcpy(&_data[offset], fragment.data.data(), fragment.data.size());
    std::memset(&_mask[offset], 0, fragment.data.size());
}

const std::vector<byte>& MaskedData::getData() const { return _data; }

const std::vector<byte>& MaskedData::getMask() const { return _mask; }
