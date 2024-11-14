#include "../../../../inc/sock/udp/data-fragmenter/ComprEncrDataFragmenter.hpp"

#include <execution>

#include "../../../../inc/utils/array/ArrayChunkSplitter.hpp"
#include "../../../../inc/utils/compression/Compressor.hpp"
#include "../../../../inc/utils/crypto/aes/AESToolkit.hpp"
#include "../../../../inc/utils/compression/CompressionToolkit.hpp"

ComprEncrDataFragmenter::ComprEncrDataFragmenter(const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor):
    DataFragmenter(fragmentDescriptor.reducePayloadSize(
        CompressionToolkit::METADATA_SIZE + AESToolkit::METADATA_SIZE)),
    _encryptor(key) { }

std::vector<std::vector<byte>> ComprEncrDataFragmenter::createFragmentPayloads(const std::vector<byte>& data) {
    const std::vector<std::vector<byte>> compressedFPs = compressFragmentPayloads(data);
    const std::vector<std::vector<byte>> encryptedFPs = encryptFragmentPayloads(compressedFPs);
    return encryptedFPs;
}

std::vector<std::vector<byte>> ComprEncrDataFragmenter::compressFragmentPayloads(const std::vector<byte>& data) const {
    const size_t fragmentDataSize = getFragmentDescriptor().getPayloadSize();
    const ArrayChunkSplitter<byte> chunkSplitter(data, fragmentDataSize);
    const size_t totalFragments = UDPToolkit::calcTotalFragments(data.size(), fragmentDataSize);
    
    std::vector<std::vector<byte>> compressedFPs(totalFragments);
    std::transform(std::execution::par, compressedFPs.begin(), compressedFPs.end(), compressedFPs.begin(),
        [&](const std::vector<byte>& fragment) {
            const size_t i = &fragment - compressedFPs.data();
            const auto [begin, len] = chunkSplitter.getChunk(i);
            return Compressor::compress(begin, len);
        }
    );
    return compressedFPs;
}

std::vector<std::vector<byte>> ComprEncrDataFragmenter::encryptFragmentPayloads(const std::vector<std::vector<byte>>& compressedFPs) {
    std::vector<std::vector<byte>> encryptedFPs(compressedFPs.size());
    for (size_t i = 0; i < compressedFPs.size(); ++i) {
        encryptedFPs[i] = _encryptor.encrypt(compressedFPs[i]);
    }
    return encryptedFPs;
}
