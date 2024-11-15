#pragma once

#include "DataFragmenter.hpp"
#include "../../../crypto/aes/AESEncryptor.hpp"
#include "../../../utils/sock/udp/PayloadMerger.hpp"
#include "../../../utils/sock/udp/UDPToolkit.hpp"

class ComprEncrDataFragmenter final : public DataFragmenter {
    using MergedChunks = PayloadMerger::MergedChunks;
    
    AESEncryptor _encryptor;
public:
    explicit ComprEncrDataFragmenter(const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor = UDPToolkit::MAX_FRAGMENT_DESCRIPTOR);
protected:
    std::vector<std::vector<byte>> splitDataIntoFragments(const std::vector<byte>& data) override;
private:
    std::vector<std::vector<byte>> compressFragmentData(const std::vector<byte>& data) const;
    MergedChunks mergeCompressedChunks(const std::vector<std::vector<byte>>& compressedChunks) const;
    std::vector<std::vector<byte>> encryptFragmentPayloads(const std::vector<std::vector<byte>>& compressedFPs);
    std::vector<std::vector<byte>> createFragments(const std::vector<std::vector<byte>>& encryptedFPs, const std::vector<size_t>& chunkCounts, const size_t totalSize) const;
};
