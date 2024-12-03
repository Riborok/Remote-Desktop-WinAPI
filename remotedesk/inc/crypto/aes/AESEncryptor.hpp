#pragma once

#include <aes.h>
#include <osrng.h>
#include <vector>

#include "AESKeyController.hpp"
#include "modes.h"
#include "../../types/byte.hpp"

class AESEncryptor final : public AESKeyController {
    using AESCtrEncryption = CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption;

    AESCtrEncryption _encryptor;
    CryptoPP::AutoSeededRandomPool _rng;
public:
    explicit AESEncryptor(const std::vector<byte>& key);
    AESEncryptor() = default;

    std::vector<byte> encrypt(const std::vector<byte>& plaintext);
    std::vector<byte> encrypt(const byte* plaintext, const size_t plaintextSize);
private:
    CryptoPP::SecByteBlock generateIV();
    static void appendIVToCiphertext(std::vector<byte>& ciphertext, const CryptoPP::SecByteBlock& iv);
    void performEncryption(std::vector<byte>& ciphertext, const byte* plaintext, const size_t plaintextSize);
};
