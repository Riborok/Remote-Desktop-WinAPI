#include "../../../inc/crypto/aes/AESEncryptor.hpp"

#include "../../../inc/utils/crypto/aes/AESToolkit.hpp"

AESEncryptor::AESEncryptor(const std::vector<byte>& key): _key(key) { }

void AESEncryptor::setKey(const std::vector<byte>& key) {
    _key = key;
}

std::vector<byte> AESEncryptor::encrypt(const std::vector<byte>& plaintext) {
    return encrypt(plaintext.data(), plaintext.size());
}

std::vector<byte> AESEncryptor::encrypt(const byte* plaintext, const size_t plaintextSize) {
    const CryptoPP::SecByteBlock iv = generateIV();
    std::vector<byte> ciphertext(plaintextSize + AESToolkit::METADATA_SIZE);
    setKeyWithIV(iv);
    appendIVToCiphertext(ciphertext, iv);
    performEncryption(ciphertext, plaintext, plaintextSize);
    return ciphertext;
}

CryptoPP::SecByteBlock AESEncryptor::generateIV() {
    CryptoPP::SecByteBlock iv(AESToolkit::METADATA_SIZE);
    _rng.GenerateBlock(iv, iv.size());
    return iv;
}

void AESEncryptor::setKeyWithIV(const CryptoPP::SecByteBlock& iv) {
    _encryptor.SetKeyWithIV(_key.data(), _key.size(), iv.data());
}

void AESEncryptor::appendIVToCiphertext(std::vector<byte>& ciphertext, const CryptoPP::SecByteBlock& iv) {
    std::memcpy(ciphertext.data(), iv.data(), iv.size());
}

void AESEncryptor::performEncryption(std::vector<byte>& ciphertext, const byte* plaintext, const size_t plaintextSize) {
    _encryptor.ProcessData(&ciphertext[AESToolkit::METADATA_SIZE], plaintext, plaintextSize);
}
