#include "../../inc/aes/AESEncryptor.hpp"

AESEncryptor::AESEncryptor(const std::vector<byte>& key): _key(key) {
}

std::vector<byte> AESEncryptor::encrypt(const std::vector<byte>& data) {
    std::vector<byte> ciphertext;
    CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption encryptor;
        
    const CryptoPP::SecByteBlock iv = generateIV();
    appendIVToCiphertext(iv, ciphertext);
    setKeyAndIV(encryptor, iv);
    performEncryption(encryptor, data, ciphertext);

    return ciphertext;
}

CryptoPP::SecByteBlock AESEncryptor::generateIV() {
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    _rng.GenerateBlock(iv, iv.size());
    return iv;
}

void AESEncryptor::appendIVToCiphertext(const CryptoPP::SecByteBlock& iv, std::vector<byte>& ciphertext) {
    ciphertext.insert(ciphertext.end(), iv.begin(), iv.end());
}

void AESEncryptor::setKeyAndIV(CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption& encryptor,
        const CryptoPP::SecByteBlock& iv) const {
    encryptor.SetKeyWithIV(_key.data(), _key.size(), iv.data());
}

void AESEncryptor::performEncryption(CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption& encryptor,
        const std::vector<byte>& plaintext, std::vector<byte>& ciphertext) {
    const size_t ciphertextStartIdx = ciphertext.size();
    ciphertext.resize(ciphertextStartIdx + plaintext.size());
    encryptor.ProcessData(&ciphertext[ciphertextStartIdx], plaintext.data(), plaintext.size());
}
