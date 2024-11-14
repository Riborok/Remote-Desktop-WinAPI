#pragma once

#include <aes.h>

class AESToolkit {
public:
    AESToolkit() = delete;
    
    static constexpr size_t METADATA_SIZE = CryptoPP::AES::BLOCKSIZE;

    static constexpr size_t DEFAULT_KEY_LENGTH = CryptoPP::AES::DEFAULT_KEYLENGTH;
    static constexpr size_t MAX_KEY_LENGTH = CryptoPP::AES::MAX_KEYLENGTH;
    static constexpr size_t MIN_KEY_LENGTH = CryptoPP::AES::MIN_KEYLENGTH;
};
