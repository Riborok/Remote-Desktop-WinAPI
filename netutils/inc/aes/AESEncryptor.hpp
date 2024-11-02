#pragma once

#include <aes.h>
#include <osrng.h>
#include <vector>
#include "modes.h"

using CryptoPP::byte;

class AESEncryptor {
    std::vector<byte> _key;
public:
    explicit AESEncryptor(const std::vector<byte>& key);
    [[nodiscard]] std::vector<byte> encrypt(const std::vector<byte>& data) const;
private:
    void checkKeySize() const;
    static CryptoPP::SecByteBlock generateIV();
    static void appendIVToCiphertext(const CryptoPP::SecByteBlock& iv, std::vector<byte>& ciphertext);
    void setKeyAndIV(CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption& encryptor,
        const CryptoPP::SecByteBlock& iv) const;
    static void performEncryption(CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption& encryptor,
        const std::vector<byte>& plaintext, std::vector<byte>& ciphertext);
};