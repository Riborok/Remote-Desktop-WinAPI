#include "../../../../inc/sock/udp/data-fragmenter/DataFragmenter.hpp"

#include "../../../../inc/utils/array/ArrayChunkSplitter.hpp"
#include "../../../../inc/utils/sock/udp/UDPToolkit.hpp"

DataFragmenter::DataFragmenter(const FragmentDescriptor& fragmentDescriptor):
    _fragmentDescriptor(fragmentDescriptor) { }

std::vector<std::vector<byte>> DataFragmenter::createFragmentPayloads(const std::vector<byte>& data) {
    const size_t fragmentDataSize = _fragmentDescriptor.getPayloadSize();
    const ArrayChunkSplitter<byte> chunkSplitter(data, fragmentDataSize);
    const size_t totalFragments = UDPToolkit::calcTotalFragments(data.size(), fragmentDataSize);
    
    std::vector<std::vector<byte>> fragmentPayloads(totalFragments);
    for (size_t i = 0; i < totalFragments; ++i) {
        const auto [begin, len] = chunkSplitter.getChunk(i);
        fragmentPayloads[i] = std::vector<byte>(begin, begin + len);
    }
    return fragmentPayloads;
}

const FragmentDescriptor& DataFragmenter::getFragmentDescriptor() const { return _fragmentDescriptor; }
