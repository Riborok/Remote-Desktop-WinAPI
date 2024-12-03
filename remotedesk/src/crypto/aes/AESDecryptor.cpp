#include "../../../inc/crypto/aes/AESDecryptor.hpp"

#include "../../../inc/utils/crypto/aes/AESToolkit.hpp"

AESDecryptor::AESDecryptor(const std::vector<byte>& key): AESKeyController(key) { }

std::vector<byte> AESDecryptor::decrypt(const std::vector<byte>& ciphertext) {
    return decrypt(ciphertext.data(), ciphertext.size());
}

std::vector<byte> AESDecryptor::decrypt(const byte* ciphertext, const size_t ciphertextSize) {
    validateCiphertextSize(ciphertextSize);
    std::vector<byte> plaintext(ciphertextSize - AESToolkit::METADATA_SIZE);
    const CryptoPP::SecByteBlock iv = extractIV(ciphertext);
    setKeyWithIV(_decryptor, iv);
    performDecryption(ciphertext, plaintext);
    return plaintext;
}

void AESDecryptor::validateCiphertextSize(const size_t ciphertextSize) {
    if (ciphertextSize < AESToolkit::METADATA_SIZE) {
        throw std::invalid_argument("Invalid ciphertext");
    }
}

CryptoPP::SecByteBlock AESDecryptor::extractIV(const byte* ciphertext) {
    CryptoPP::SecByteBlock iv(AESToolkit::METADATA_SIZE);
    std::memcpy(iv.data(), ciphertext, iv.size());
    return iv;
}

void AESDecryptor::performDecryption(const byte* ciphertext, std::vector<byte>& plaintext) {
    _decryptor.ProcessData(plaintext.data(), &ciphertext[AESToolkit::METADATA_SIZE], plaintext.size());
}
