#include "../../inc/aes/AESEncryptor.hpp"

AESEncryptor::AESEncryptor(std::vector<byte>&& key): _key(std::move(key)) { }

void AESEncryptor::setKey(std::vector<byte>&& key) {
    _key = std::move(key);
}

std::vector<byte> AESEncryptor::encrypt(const std::vector<byte>& plaintext) {
    return encryptWithIV(plaintext.data(), plaintext.size());
}

std::vector<byte> AESEncryptor::encrypt(const byte* plaintext, const size_t plaintextLength) {
    return encryptWithIV(plaintext, plaintextLength);
}

std::vector<byte> AESEncryptor::encryptWithIV(const byte* plaintext, const size_t plaintextLength) {
    const CryptoPP::SecByteBlock iv = generateIV();
    std::vector<byte> ciphertext(getCiphertextSize(plaintextLength));
    setKeyWithIV(iv);
    appendIVToCiphertext(ciphertext, iv);
    performEncryption(ciphertext, plaintext, plaintextLength);
    return ciphertext;
}

CryptoPP::SecByteBlock AESEncryptor::generateIV() {
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    _rng.GenerateBlock(iv, iv.size());
    return iv;
}

void AESEncryptor::setKeyWithIV(const CryptoPP::SecByteBlock& iv) {
    _encryptor.SetKeyWithIV(_key.data(), _key.size(), iv.data());
}

void AESEncryptor::appendIVToCiphertext(std::vector<byte>& ciphertext, const CryptoPP::SecByteBlock& iv) {
    std::memcpy(ciphertext.data(), iv.data(), iv.size());
}

void AESEncryptor::performEncryption(std::vector<byte>& ciphertext, const byte* plaintext, const size_t plaintextLength) {
    _encryptor.ProcessData(&ciphertext[CryptoPP::AES::BLOCKSIZE], plaintext, plaintextLength);
}

size_t AESEncryptor::getCiphertextSize(const size_t plaintextSize) {
    return plaintextSize + CryptoPP::AES::BLOCKSIZE;
}
