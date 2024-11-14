#pragma once

#include <aes.h>
#include <vector>
#include "modes.h"
#include "../../types/byte.hpp"

class AESDecryptor {
    using AESCtrDecryption = CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption;

    AESCtrDecryption _decryptor;
    std::vector<byte> _key;
public:
    explicit AESDecryptor(const std::vector<byte>& key);
    AESDecryptor() = default;
    
    void setKey(const std::vector<byte>& key);
    std::vector<byte> decrypt(const std::vector<byte>& ciphertext);
    std::vector<byte> decrypt(const byte* ciphertext, const size_t ciphertextSize);
private:
    static void validateCiphertextSize(const size_t ciphertextSize);
    static CryptoPP::SecByteBlock extractIV(const byte* ciphertext);
    void setKeyWithIV(const CryptoPP::SecByteBlock& iv);
    void performDecryption(const byte* ciphertext, std::vector<byte>& plaintext);
};
