#pragma once

#include "DataFragmenter.hpp"
#include "../../../crypto/aes/AESEncryptor.hpp"
#include "../../../utils/sock/udp/UDPToolkit.hpp"

class ComprEncrDataFragmenter final : public DataFragmenter {
    AESEncryptor _encryptor;
public:
    explicit ComprEncrDataFragmenter(const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor = UDPToolkit::MAX_FRAGMENT_DESCRIPTOR);
    std::vector<std::vector<byte>> createFragmentPayloads(const std::vector<byte>& data) override;
private:
    std::vector<std::vector<byte>> compressFragmentPayloads(const std::vector<byte>& data) const;
    std::vector<std::vector<byte>> encryptFragmentPayloads(const std::vector<std::vector<byte>>& compressedFPs);
};
