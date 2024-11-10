#include "../../../../inc/sock/udp/data-fragmenter/DataFragmenter.hpp"

#include "../../../../inc/utils/sock/udp/UDPToolkit.hpp"

DataFragmenter::DataFragmenter(const FragmentDescriptor& fragmentDescriptor):
    _fragmentDescriptor(fragmentDescriptor) { }

std::vector<std::vector<byte>> DataFragmenter::createDataFragments(const std::vector<byte>& data) {
    const size_t totalFragments = UDPToolkit::calcTotalFragments(data.size(), _fragmentDescriptor.getDataSize());
    std::vector<std::vector<byte>> dataFragments(totalFragments);
    for (size_t i = 0; i < totalFragments; ++i) {
        const auto [begin, len] = getDataFragment(data, i);
        dataFragments[i] = std::vector<byte>(begin, begin + len);
    }
    return dataFragments;
}

const FragmentDescriptor& DataFragmenter::getFragmentDescriptor() const { return _fragmentDescriptor; }

std::tuple<const byte*, size_t> DataFragmenter::getDataFragment(const std::vector<byte>& data, const size_t i) const {
    const size_t offset = i * _fragmentDescriptor.getDataSize();
    const size_t dataLen = std::min(_fragmentDescriptor.getDataSize(), data.size() - offset);
    return std::make_tuple(&data[offset], dataLen);
}
