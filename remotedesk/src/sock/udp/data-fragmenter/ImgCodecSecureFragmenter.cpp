#include "../../../../inc/sock/udp/data-fragmenter/ImgCodecSecureFragmenter.hpp"

#include "../../../../inc/utils/crypto/aes/AESToolkit.hpp"
#include "../../../../inc/utils/sock/udp/PayloadMerger.hpp"

ImgCodecSecureFragmenter::ImgCodecSecureFragmenter(const SIZE& size, const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor):
    DataFragmenter(
        fragmentDescriptor
            .reducePayloadSize(AESToolkit::METADATA_SIZE)
            .reduceDataSize(PayloadMerger::PREFIX_SIZE)
    ),
    _imageTileSplitter(size),
    _encryptor(key) { }

std::vector<std::vector<byte>> ImgCodecSecureFragmenter::splitDataIntoFragments(const std::vector<byte>& data) {
    const std::vector<std::vector<byte>> tiles = _imageTileSplitter.splitToTiles(data);
    const MergedChunks mergedChunks = mergeCompressedChunks(tiles);
    const std::vector<std::vector<byte>> encryptedFPs = encryptFragmentPayloads(mergedChunks.payloads);
    return createFragments(encryptedFPs, mergedChunks.chunkCounts, data.size());
}

ImgCodecSecureFragmenter::MergedChunks ImgCodecSecureFragmenter::mergeCompressedChunks(const std::vector<std::vector<byte>>& compressedChunks) const {
    return PayloadMerger::mergeIntoPayloads(compressedChunks, getFragmentDescriptor().getPayloadSize());
}

std::vector<std::vector<byte>> ImgCodecSecureFragmenter::encryptFragmentPayloads(const std::vector<std::vector<byte>>& compressedFPs) {
    std::vector<std::vector<byte>> encryptedFPs(compressedFPs.size());
    for (size_t i = 0; i < compressedFPs.size(); ++i) {
        encryptedFPs[i] = _encryptor.encrypt(compressedFPs[i]);
    }
    return encryptedFPs;
}

std::vector<std::vector<byte>> ImgCodecSecureFragmenter::createFragments(const std::vector<std::vector<byte>>& encryptedFPs,
        const std::vector<size_t>& chunkCounts, const size_t totalSize) const {
    const size_t fragmentId = getId();
    std::vector<std::vector<byte>> fragments(encryptedFPs.size());
    for (size_t fragmentNumber = 0, i = 0; i < encryptedFPs.size(); ++i) {
        fragments[i] = UDPToolkit::createFragment(encryptedFPs[i], {fragmentId, fragmentNumber, totalSize});
        fragmentNumber += chunkCounts[i];
    }
    return fragments;
}
