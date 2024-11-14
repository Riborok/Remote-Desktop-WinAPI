#include "../../../../inc/sock/udp/data-fragmenter/DataFragmenter.hpp"

#include "../../../../inc/utils/array/ArrayChunkSplitter.hpp"
#include "../../../../inc/utils/sock/udp/UDPToolkit.hpp"

DataFragmenter::DataFragmenter(const FragmentDescriptor& fragmentDescriptor):
    _fragmentDescriptor(fragmentDescriptor) { }

std::vector<std::vector<byte>> DataFragmenter::createDataFragments(const std::vector<byte>& data) {
    const ArrayChunkSplitter<byte> dataChunker(data, _fragmentDescriptor.getDataSize());
    const size_t totalFragments = UDPToolkit::calcTotalFragments(data.size(), _fragmentDescriptor.getDataSize());
    
    std::vector<std::vector<byte>> dataFragments(totalFragments);
    for (size_t i = 0; i < totalFragments; ++i) {
        const auto [begin, len] = dataChunker.getChunk(i);
        dataFragments[i] = std::vector<byte>(begin, begin + len);
    }
    return dataFragments;
}

const FragmentDescriptor& DataFragmenter::getFragmentDescriptor() const { return _fragmentDescriptor; }
