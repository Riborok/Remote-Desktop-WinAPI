#pragma once

#include "DataReassembler.hpp"
#include "../../../aes/AESDecryptor.hpp"

class DDDataReassembler final : public DataReassembler {
    AESDecryptor _decryptor;
public:
    explicit DDDataReassembler(std::vector<byte>&& key);
    MaskedData reassembleData(std::vector<Fragment>& fragments) override;
private:
    void decryptDataFragments(std::vector<Fragment>& fragments);
    static void decompressDataFragments(std::vector<Fragment>& decryptedFragments);
};
