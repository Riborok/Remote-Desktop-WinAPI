#include "../../../../inc/sock/udp/data-fragmenter/DataFragmenter.hpp"

#include "../../../../inc/utils/array/ArrayChunkSplitter.hpp"
#include "../../../../inc/utils/sock/udp/UDPToolkit.hpp"

DataFragmenter::DataFragmenter(const FragmentDescriptor& fragmentDescriptor):
    _fragmentDescriptor(fragmentDescriptor) { }

std::vector<std::vector<byte>> DataFragmenter::createFragments(const std::vector<byte>& data) {
    std::vector<std::vector<byte>> fragments = splitDataIntoFragments(data);
    ++_id;
    return fragments;
}

std::vector<std::vector<byte>> DataFragmenter::splitDataIntoFragments(const std::vector<byte>& data) {
    const std::vector<std::vector<byte>> fragmentPayloads = createFragmentPayloads(data);
    return createFragments(fragmentPayloads, data.size());
}

std::vector<std::vector<byte>> DataFragmenter::createFragmentPayloads(const std::vector<byte>& data) const {
    const size_t fragmentDataSize = _fragmentDescriptor.getDataSize();
    const ArrayChunkSplitter<byte> chunkSplitter(data, fragmentDataSize);
    const size_t totalFragments = UDPToolkit::calcTotalFragments(data.size(), fragmentDataSize);
    
    std::vector<std::vector<byte>> fragmentPayloads(totalFragments);
    for (size_t i = 0; i < totalFragments; ++i) {
        const auto [begin, len] = chunkSplitter.getChunk(i);
        fragmentPayloads[i] = std::vector<byte>(begin, begin + len);
    }
    return fragmentPayloads;
}

std::vector<std::vector<byte>> DataFragmenter::createFragments(const std::vector<std::vector<byte>>& fragmentPayloads, const size_t totalSize) const {
    const size_t totalFragments = fragmentPayloads.size();
    std::vector<std::vector<byte>> fragments(totalFragments);
    for (size_t i = 0; i < totalFragments; ++i) {
        fragments[i] = UDPToolkit::createFragment(fragmentPayloads[i], {_id, i, totalSize, totalFragments});
    }
    return fragments;
}

size_t DataFragmenter::getId() const { return _id; }

const FragmentDescriptor& DataFragmenter::getFragmentDescriptor() const { return _fragmentDescriptor; }
