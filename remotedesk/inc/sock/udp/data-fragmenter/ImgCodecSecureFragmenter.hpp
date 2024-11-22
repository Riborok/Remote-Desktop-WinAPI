#pragma once

#include "DataFragmenter.hpp"
#include "../../../crypto/aes/AESEncryptor.hpp"
#include "../../../img/ImageTileSplitter.hpp"
#include "../../../utils/sock/udp/PayloadMerger.hpp"
#include "../../../utils/sock/udp/UDPToolkit.hpp"

class ImgCodecSecureFragmenter final : public DataFragmenter {
    using MergedChunks = PayloadMerger::MergedChunks;

    ImageTileSplitter _imageTileSplitter;
    AESEncryptor _encryptor;
public:
    explicit ImgCodecSecureFragmenter(const SIZE& size, const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor = UDPToolkit::MAX_FRAGMENT_DESCRIPTOR);
protected:
    std::vector<std::vector<byte>> splitDataIntoFragments(const std::vector<byte>& data) override;
private:
    MergedChunks mergeCompressedChunks(const std::vector<std::vector<byte>>& compressedChunks) const;
    std::vector<std::vector<byte>> encryptFragmentPayloads(const std::vector<std::vector<byte>>& compressedFPs);
    std::vector<std::vector<byte>> createFragments(const std::vector<std::vector<byte>>& encryptedFPs, const std::vector<size_t>& chunkCounts, const size_t totalSize) const;
};
