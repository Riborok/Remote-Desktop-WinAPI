#include "../../../../inc/sock/udp/data-fragmenter/ComprEncrDataFragmenter.hpp"

#include <execution>

#include "../../../../inc/utils/array/ArrayChunkSplitter.hpp"
#include "../../../../inc/utils/compression/Compressor.hpp"
#include "../../../../inc/utils/crypto/aes/AESToolkit.hpp"
#include "../../../../inc/utils/compression/CompressionToolkit.hpp"
#include "../../../../inc/utils/sock/udp/PayloadMerger.hpp"

ComprEncrDataFragmenter::ComprEncrDataFragmenter(const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor):
    DataFragmenter(
        fragmentDescriptor
            .reducePayloadSize(CompressionToolkit::METADATA_SIZE + AESToolkit::METADATA_SIZE)
            .reduceDataSize(PayloadMerger::PREFIX_SIZE)
    ),
    _encryptor(key) { }

std::vector<std::vector<byte>> ComprEncrDataFragmenter::splitDataIntoFragments(const std::vector<byte>& data) {
    const std::vector<std::vector<byte>> compressedChunks = compressFragmentData(data);
    const MergedChunks mergedChunks = mergeCompressedChunks(compressedChunks);
    const std::vector<std::vector<byte>> encryptedFPs = encryptFragmentPayloads(mergedChunks.payloads);
    return createFragments(encryptedFPs, mergedChunks.chunkCounts, data.size());
}

std::vector<std::vector<byte>> ComprEncrDataFragmenter::compressFragmentData(const std::vector<byte>& data) const {
    const size_t fragmentDataSize = getFragmentDescriptor().getDataSize();
    const ArrayChunkSplitter<byte> chunkSplitter(data, fragmentDataSize);
    const size_t totalFragments = UDPToolkit::calcTotalFragments(data.size(), fragmentDataSize);
    
    std::vector<std::vector<byte>> compressedChunks(totalFragments);
    std::transform(std::execution::par, compressedChunks.begin(), compressedChunks.end(), compressedChunks.begin(),
        [&](const std::vector<byte>& fragment) {
            const size_t i = &fragment - compressedChunks.data();
            const auto [begin, len] = chunkSplitter.getChunk(i);
            return Compressor::compress(begin, len);
        }
    );
    return compressedChunks;
}

ComprEncrDataFragmenter::MergedChunks ComprEncrDataFragmenter::mergeCompressedChunks(const std::vector<std::vector<byte>>& compressedChunks) const {
    return PayloadMerger::mergeIntoPayloads(compressedChunks, getFragmentDescriptor().getPayloadSize());
}

std::vector<std::vector<byte>> ComprEncrDataFragmenter::encryptFragmentPayloads(const std::vector<std::vector<byte>>& compressedFPs) {
    std::vector<std::vector<byte>> encryptedFPs(compressedFPs.size());
    for (size_t i = 0; i < compressedFPs.size(); ++i) {
        encryptedFPs[i] = _encryptor.encrypt(compressedFPs[i]);
    }
    return encryptedFPs;
}

std::vector<std::vector<byte>> ComprEncrDataFragmenter::createFragments(const std::vector<std::vector<byte>>& encryptedFPs,
        const std::vector<size_t>& chunkCounts, const size_t totalSize) const {
    const size_t fragmentId = getId();
    std::vector<std::vector<byte>> fragments(encryptedFPs.size());
    for (size_t fragmentNumber = 0, i = 0; i < encryptedFPs.size(); ++i) {
        fragments[i] = UDPToolkit::createFragment(encryptedFPs[i], {fragmentId, fragmentNumber, totalSize});
        fragmentNumber += chunkCounts[i];
    }
    return fragments;
}
