﻿#pragma once

#include "DataReassembler.hpp"
#include "../../../aes/AESDecryptor.hpp"

class DDDataReassembler final : public DataReassembler {
    AESDecryptor _decryptor;
public:
    explicit DDDataReassembler(const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor = UDPToolkit::MAX_FRAGMENT_DESCRIPTOR);
    MaskedData reassembleData(std::vector<Fragment>& fragments) override;
private:
    void decryptDataFragments(std::vector<Fragment>& fragments);
    static void decompressDataFragments(std::vector<Fragment>& decryptedFragments);
};
