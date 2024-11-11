#pragma once

#include <aes.h>
#include <osrng.h>
#include <vector>
#include "modes.h"
#include "../types/byte.hpp"

class AESEncryptor {
    using AESCtrEncryption = CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption;

    AESCtrEncryption _encryptor;
    CryptoPP::AutoSeededRandomPool _rng;
    std::vector<byte> _key;
public:
    explicit AESEncryptor(const std::vector<byte>& key);
    AESEncryptor() = default;
    
    void setKey(const std::vector<byte>& key);
    std::vector<byte> encrypt(const std::vector<byte>& plaintext);
    std::vector<byte> encrypt(const byte* plaintext, const size_t plaintextSize);
private:
    CryptoPP::SecByteBlock generateIV();
    void setKeyWithIV(const CryptoPP::SecByteBlock& iv);
    static void appendIVToCiphertext(std::vector<byte>& ciphertext, const CryptoPP::SecByteBlock& iv);
    void performEncryption(std::vector<byte>& ciphertext, const byte* plaintext, const size_t plaintextSize);
};
