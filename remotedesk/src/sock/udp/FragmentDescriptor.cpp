#include "../../../inc/sock/udp/FragmentDescriptor.hpp"

#include "../../../inc/utils/sock/udp/UDPToolkit.hpp"

FragmentDescriptor::FragmentDescriptor(const size_t size, const size_t payloadSize):
    _size(size),
    _payloadSize(payloadSize) { }

FragmentDescriptor::FragmentDescriptor(const size_t size):
    FragmentDescriptor(size, size - UDPToolkit::METADATA_SIZE) { }

size_t FragmentDescriptor::getSize() const { return _size; }

size_t FragmentDescriptor::getPayloadSize() const { return _payloadSize; }

FragmentDescriptor FragmentDescriptor::reducePayloadSize(const size_t delta) const {
    return {_size, _payloadSize >= delta ? _payloadSize - delta : 0};
}
