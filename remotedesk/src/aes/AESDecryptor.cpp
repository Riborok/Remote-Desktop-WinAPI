#include "../../inc/aes/AESDecryptor.hpp"

AESDecryptor::AESDecryptor(std::vector<byte>&& key): _key(std::move(key)) { }

void AESDecryptor::setKey(std::vector<byte>&& key) {
    _key = std::move(key);
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
    if (ciphertext.size() < CryptoPP::AES::BLOCKSIZE) {
        throw std::invalid_argument("Invalid ciphertext");
    }
}

void AESDecryptor::decryptWithIV(const std::vector<byte>& ciphertext, byte* plaintext) {
    const CryptoPP::SecByteBlock iv = extractIV(ciphertext);
    setKeyWithIV(iv);
    performDecryption(ciphertext, plaintext);
}

CryptoPP::SecByteBlock AESDecryptor::extractIV(const std::vector<byte>& ciphertext) {
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    std::copy_n(ciphertext.begin(), iv.size(), iv.begin());
    return iv;
}

void AESDecryptor::setKeyWithIV(const CryptoPP::SecByteBlock& iv) {
    _decryptor.SetKeyWithIV(_key.data(), _key.size(), iv.data());
}

void AESDecryptor::performDecryption(const std::vector<byte>& ciphertext, byte* plaintext) {
    _decryptor.ProcessData(plaintext, &ciphertext[CryptoPP::AES::BLOCKSIZE],
        getPlaintextSize(ciphertext.size()));
}

size_t AESDecryptor::getPlaintextSize(const size_t ciphertextSize) {
    return ciphertextSize - CryptoPP::AES::BLOCKSIZE;
}
