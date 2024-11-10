#pragma once

#include <aes.h>
#include <osrng.h>
#include <vector>
#include "modes.h"

using CryptoPP::byte;

class AESDecryptor {
    using AESCtrDecryption = CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption;

    AESCtrDecryption _decryptor;
    std::vector<byte> _key;
public:
    explicit AESDecryptor(const std::vector<byte>& key);
    AESDecryptor() = default;
    
    void setKey(const std::vector<byte>& key);
    std::vector<byte> decrypt(const std::vector<byte>& ciphertext);
    void decrypt(const std::vector<byte>& ciphertext, byte* plaintext);
private:
    static void validateCiphertextLength(const std::vector<byte>& ciphertext);
    void decryptWithIV(const std::vector<byte>& ciphertext, byte* plaintext);
    static CryptoPP::SecByteBlock extractIV(const std::vector<byte>& ciphertext);
    void setKeyWithIV(const CryptoPP::SecByteBlock& iv);
    void performDecryption(const std::vector<byte>& ciphertext, byte* plaintext);
    static size_t getPlaintextSize(const size_t ciphertextSize);
};
