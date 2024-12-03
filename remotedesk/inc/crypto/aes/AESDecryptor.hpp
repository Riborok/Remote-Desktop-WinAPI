#pragma once

#include <aes.h>
#include <vector>

#include "AESKeyController.hpp"
#include "modes.h"
#include "../../types/byte.hpp"

class AESDecryptor final : public AESKeyController {
    using AESCtrDecryption = CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption;

    AESCtrDecryption _decryptor;
public:
    explicit AESDecryptor(const std::vector<byte>& key);
    AESDecryptor() = default;

    std::vector<byte> decrypt(const std::vector<byte>& ciphertext);
    std::vector<byte> decrypt(const byte* ciphertext, const size_t ciphertextSize);
private:
    static void validateCiphertextSize(const size_t ciphertextSize);
    static CryptoPP::SecByteBlock extractIV(const byte* ciphertext);
    void performDecryption(const byte* ciphertext, std::vector<byte>& plaintext);
};
