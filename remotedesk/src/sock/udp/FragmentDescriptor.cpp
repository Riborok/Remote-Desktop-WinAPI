#include "../../../inc/sock/udp/FragmentDescriptor.hpp"

#include "../../../inc/utils/sock/udp/UDPToolkit.hpp"

FragmentDescriptor::FragmentDescriptor(const size_t size, const size_t payloadSize, const size_t dataSize):
    _size(size),
    _payloadSize(payloadSize),
    _dataSize(dataSize) { }

FragmentDescriptor::FragmentDescriptor(const size_t size, const size_t payloadSize):
    FragmentDescriptor(size, payloadSize, payloadSize) {}

FragmentDescriptor::FragmentDescriptor(const size_t size):
    FragmentDescriptor(size, size - UDPToolkit::METADATA_SIZE) { }

size_t FragmentDescriptor::getSize() const { return _size; }

size_t FragmentDescriptor::getPayloadSize() const { return _payloadSize; }

size_t FragmentDescriptor::getDataSize() const { return _dataSize; }

FragmentDescriptor FragmentDescriptor::reducePayloadSize(const size_t delta) const {
    return {_size, _payloadSize >= delta ? _payloadSize - delta : 0};
}

FragmentDescriptor FragmentDescriptor::reduceDataSize(const size_t delta) const {
    return {_size, _payloadSize, _dataSize >= delta ? _dataSize - delta : 0};
}
