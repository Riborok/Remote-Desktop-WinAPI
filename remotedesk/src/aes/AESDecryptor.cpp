#include "../../inc/aes/AESDecryptor.hpp"

#include "../../inc/utils/aes/AESToolkit.hpp"

AESDecryptor::AESDecryptor(const std::vector<byte>& key): _key(key) { }

void AESDecryptor::setKey(const std::vector<byte>& key) {
    _key = key;
}

std::vector<byte> AESDecryptor::decrypt(const std::vector<byte>& ciphertext) {
    validateCiphertextLength(ciphertext);
    std::vector<byte> plaintext(getPlaintextSize(ciphertext.size()));
    decryptWithIV(ciphertext, plaintext.data());
    return plaintext;
}

void AESDecryptor::decrypt(const std::vector<byte>& ciphertext, byte* plaintext) {
    validateCiphertextLength(ciphertext);
    decryptWithIV(ciphertext, plaintext);
}

void AESDecryptor::validateCiphertextLength(const std::vector<byte>& ciphertext) {
    if (ciphertext.size() < AESToolkit::METADATA_SIZE) {
        throw std::invalid_argument("Invalid ciphertext");
    }
}

void AESDecryptor::decryptWithIV(const std::vector<byte>& ciphertext, byte* plaintext) {
    const CryptoPP::SecByteBlock iv = extractIV(ciphertext);
    setKeyWithIV(iv);
    performDecryption(ciphertext, plaintext);
}

CryptoPP::SecByteBlock AESDecryptor::extractIV(const std::vector<byte>& ciphertext) {
    CryptoPP::SecByteBlock iv(AESToolkit::METADATA_SIZE);
    std::copy_n(ciphertext.begin(), iv.size(), iv.begin());
    return iv;
}

void AESDecryptor::setKeyWithIV(const CryptoPP::SecByteBlock& iv) {
    _decryptor.SetKeyWithIV(_key.data(), _key.size(), iv.data());
}

void AESDecryptor::performDecryption(const std::vector<byte>& ciphertext, byte* plaintext) {
    _decryptor.ProcessData(plaintext, &ciphertext[AESToolkit::METADATA_SIZE],
        getPlaintextSize(ciphertext.size()));
}

size_t AESDecryptor::getPlaintextSize(const size_t ciphertextSize) {
    return ciphertextSize - AESToolkit::METADATA_SIZE;
}
