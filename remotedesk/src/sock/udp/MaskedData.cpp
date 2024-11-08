#include "../../../inc/sock/udp/MaskedData.hpp"

#include "../../../inc/sock/udp/UDPSender.hpp"
#include "../../../inc/utils/TypeLimits.hpp"

void MaskedData::resize(const size_t size) {
    _data.resize(size, 0);
    _mask.resize(size, TypeLimits::MAX_BYTE);
}

void MaskedData::insertPayload(const Payload& payload) {
    const size_t offset = payload.fragmentNumber * UDPSender::DATA_SIZE;
    std::memcpy(&_data[offset], payload.data.data(), payload.data.size());
    std::memset(&_mask[offset], 0, payload.data.size());
}

const std::vector<byte>& MaskedData::getData() const { return _data; }

const std::vector<byte>& MaskedData::getMask() const { return _mask; }
