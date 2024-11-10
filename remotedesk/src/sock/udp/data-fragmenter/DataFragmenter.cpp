#include "../../../../inc/sock/udp/data-fragmenter/DataFragmenter.hpp"

#include "../../../../inc/utils/sock/udp/UDPToolkit.hpp"

std::vector<std::vector<byte>> DataFragmenter::createDataFragments(const std::vector<byte>& data) {
    const size_t totalFragments = UDPToolkit::calcTotalFragments(data.size());
    std::vector<std::vector<byte>> dataFragments(totalFragments);
    for (size_t i = 0; i < totalFragments; ++i) {
        const auto [begin, len] = getDataFragment(data, i);
        dataFragments[i] = std::vector<byte>(begin, begin + len);
    }
    return dataFragments;
}

std::tuple<const byte*, size_t> DataFragmenter::getDataFragment(const std::vector<byte>& data, const size_t i) {
    const size_t offset = i * UDPToolkit::DATA_SIZE;
    const size_t dataLen = std::min(UDPToolkit::DATA_SIZE, data.size() - offset);
    return std::make_tuple(&data[offset], dataLen);
}
