#pragma once

#include <aes.h>

class AESToolkit {
public:
    AESToolkit() = delete;
    
    static constexpr size_t METADATA_SIZE = CryptoPP::AES::BLOCKSIZE;
};
