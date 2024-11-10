#pragma once

#include "DataFragmenter.hpp"
#include "../../../aes/AESEncryptor.hpp"
#include "../../../utils/sock/udp/UDPToolkit.hpp"

class CEDataFragmenter final : public DataFragmenter {
    AESEncryptor _encryptor;
public:
    explicit CEDataFragmenter(std::vector<byte>&& key);
    std::vector<std::vector<byte>> createDataFragments(const std::vector<byte>& data) override;
private:
    static std::vector<std::vector<byte>> compressDataFragments(const std::vector<byte>& data);
    std::vector<std::vector<byte>> encryptDataFragments(const std::vector<std::vector<byte>>& compressedDFs);
};
