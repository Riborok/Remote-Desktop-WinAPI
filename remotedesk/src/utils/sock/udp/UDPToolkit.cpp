#include "../../../../inc/utils/sock/udp/UDPToolkit.hpp"

#include "../../../../inc/utils/ByteArrayUtils.hpp"
#include "../../../../inc/utils/MathUtils.hpp"

size_t UDPToolkit::calcTotalFragments(const size_t dataSize) {
    return MathUtils::ceil(dataSize, DATA_SIZE);
}

std::vector<byte> UDPToolkit::createFragment(const std::vector<byte>& dataFragment, const Metadata& metadata) {
    std::vector<byte> fragment(dataFragment.size() + METADATA_SIZE);
    addData(fragment, dataFragment);
    addId(fragment, metadata.id);
    addFragmentNumber(fragment, metadata.fragmentNumber);
    addTotalSize(fragment, metadata.totalDataSize);
    return fragment;
}

void UDPToolkit::addData(std::vector<byte>& fragment, const std::vector<byte>& data) {
    std::memcpy(fragment.data(), data.data(), data.size());
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

void UDPToolkit::addSizeTToFragment(std::vector<byte>& fragment, const size_t value, const size_t startIdx) {
    ByteArrayUtils::setValueByByteIndex(fragment, startIdx, value);
}

void UDPToolkit::populateFragment(Fragment& fragment, const int bytesReceived) {
    fragment.totalDataSize = extractTotalSize(fragment.data, bytesReceived);
    fragment.fragmentNumber = extractFragmentNumber(fragment.data, bytesReceived);
    fragment.id = extractId(fragment.data, bytesReceived);
    fragment.data.resize(bytesReceived - METADATA_SIZE);
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

size_t UDPToolkit::extractSizeTFromFragment(const std::vector<byte>& fragment, const size_t startIdx) {
    return ByteArrayUtils::getValueByByteIndex<size_t>(fragment, startIdx);
}
