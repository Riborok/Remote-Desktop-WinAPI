#pragma once

#include "DataReassembler.hpp"
#include "../../../crypto/aes/AESDecryptor.hpp"

class DecrDecomprDataReassembler final : public DataReassembler {
    AESDecryptor _decryptor;
public:
    explicit DecrDecomprDataReassembler(const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor = UDPToolkit::MAX_FRAGMENT_DESCRIPTOR);
    MaskedData reassembleData(std::vector<Fragment>& fragments) override;
private:
    void decryptFragmentPayloads(std::vector<Fragment>& fragments);
    static void splitIntoChunks(std::vector<Fragment>& fragments);
    static std::vector<Fragment> createFragmentChunks(const std::vector<Fragment>& fragments);
    static void addFragmentChunks(std::vector<Fragment>& chunksOfFragments, const Fragment& fragment);
    static void decompressFragmentPayloads(std::vector<Fragment>& decryptedFragments);
};
