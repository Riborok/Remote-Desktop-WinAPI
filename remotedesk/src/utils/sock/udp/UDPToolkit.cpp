#include "../../../../inc/utils/sock/udp/UDPToolkit.hpp"

#include "../../../../inc/utils/array/ByteArrayUtils.hpp"
#include "../../../../inc/utils/MathUtils.hpp"

const FragmentDescriptor UDPToolkit::MAX_FRAGMENT_DESCRIPTOR = FragmentDescriptor(MAX_FRAGMENT_SIZE);
const FragmentDescriptor UDPToolkit::MTU_FRAGMENT_DESCRIPTOR = FragmentDescriptor(MTU_FRAGMENT_SIZE);

size_t UDPToolkit::calcTotalFragments(const size_t payloadSize, const size_t fragmentPayloadSize) {
    return MathUtils::ceil(payloadSize, fragmentPayloadSize);
}

std::vector<byte> UDPToolkit::createFragment(const std::vector<byte>& fragmentPayload, const Metadata& metadata) {
    std::vector<byte> fragment(fragmentPayload.size() + METADATA_SIZE);
    addPayload(fragment, fragmentPayload);
    addId(fragment, metadata.fragmentId);
    addFragmentNumber(fragment, metadata.fragmentNumber);
    addTotalSize(fragment, metadata.totalDataSize);
    return fragment;
}

void UDPToolkit::addPayload(std::vector<byte>& fragment, const std::vector<byte>& payload) {
    std::memcpy(fragment.data(), payload.data(), payload.size());
}

void UDPToolkit::addTotalSize(std::vector<byte>& fragment, const size_t totalSize) {
    addSizeTToFragment(fragment, totalSize, fragment.size() - sizeof(size_t));
}

void UDPToolkit::addFragmentNumber(std::vector<byte>& fragment, const size_t fragmentNumber) {
    addSizeTToFragment(fragment, fragmentNumber, fragment.size() - 2*sizeof(size_t));
}

void UDPToolkit::addId(std::vector<byte>& fragment, const size_t id) {
    addSizeTToFragment(fragment, id, fragment.size() - 3*sizeof(size_t));
}

void UDPToolkit::addSizeTToFragment(std::vector<byte>& fragment, const size_t value, const size_t index) {
    ByteArrayUtils::setValueByByteIndex(fragment, index, value);
}

void UDPToolkit::populateFragment(Fragment& fragment, const int bytesReceived) {
    fragment.totalDataSize = extractTotalSize(fragment.payload, bytesReceived);
    fragment.fragmentNumber = extractFragmentNumber(fragment.payload, bytesReceived);
    fragment.fragmentId = extractId(fragment.payload, bytesReceived);
    fragment.payload.resize(bytesReceived - METADATA_SIZE);
}

size_t UDPToolkit::extractTotalSize(const std::vector<byte>& fragment, const int bytesReceived) {
    return extractSizeTFromFragment(fragment, bytesReceived - sizeof(size_t));
}

size_t UDPToolkit::extractFragmentNumber(const std::vector<byte>& fragment, const int bytesReceived) {
    return extractSizeTFromFragment(fragment, bytesReceived - 2*sizeof(size_t));
}

size_t UDPToolkit::extractId(const std::vector<byte>& fragment, const int bytesReceived) {
    return extractSizeTFromFragment(fragment, bytesReceived - 3*sizeof(size_t));
}

size_t UDPToolkit::extractSizeTFromFragment(const std::vector<byte>& fragment, const size_t index) {
    return ByteArrayUtils::getValueByByteIndex<size_t>(fragment, index);
}
