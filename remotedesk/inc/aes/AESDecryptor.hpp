#pragma once

#include <aes.h>
#include <osrng.h>
#include <vector>
#include "modes.h"

using CryptoPP::byte;

class AESDecryptor {
    std::vector<byte> _key;
public:
    explicit AESDecryptor(const std::vector<byte>& key);
    std::vector<byte> decrypt(const std::vector<byte>& ciphertext) const;
private:
    static void validateCiphertextLength(const std::vector<byte>& ciphertext);
    static CryptoPP::SecByteBlock extractIV(const std::vector<byte>& ciphertext);
    void setKeyAndIV(CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption& decryptor,
        const CryptoPP::SecByteBlock& iv) const;
    static void performDecryption(CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption& decryptor,
        const std::vector<byte>& ciphertext, std::vector<byte>& decryptedData);
};
