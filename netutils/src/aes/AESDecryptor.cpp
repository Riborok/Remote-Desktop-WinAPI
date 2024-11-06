#include "../../inc/aes/AESDecryptor.hpp"

AESDecryptor::AESDecryptor(const std::vector<byte>& key): _key(key) {
}

std::vector<byte> AESDecryptor::decrypt(const std::vector<byte>& ciphertext) const {
    validateCiphertextLength(ciphertext);
    std::vector<byte> decryptedData;
    CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption decryptor;

    const CryptoPP::SecByteBlock iv = extractIV(ciphertext);
    setKeyAndIV(decryptor, iv);
    performDecryption(decryptor, ciphertext, decryptedData);

    return decryptedData;
}

void AESDecryptor::validateCiphertextLength(const std::vector<byte>& ciphertext) {
    if (ciphertext.size() < CryptoPP::AES::BLOCKSIZE) {
        throw std::invalid_argument("Invalid ciphertext");
    }
}

CryptoPP::SecByteBlock AESDecryptor::extractIV(const std::vector<byte>& ciphertext) {
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    std::copy_n(ciphertext.begin(), iv.size(), iv.begin());
    return iv;
}

void AESDecryptor::setKeyAndIV(CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption& decryptor,
        const CryptoPP::SecByteBlock& iv) const {
    decryptor.SetKeyWithIV(_key.data(), _key.size(), iv.data());
}

void AESDecryptor::performDecryption(CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption& decryptor,
        const std::vector<byte>& ciphertext, std::vector<byte>& decryptedData) {
    const size_t ciphertextSize = ciphertext.size() - CryptoPP::AES::BLOCKSIZE;
    decryptedData.resize(ciphertextSize);
    decryptor.ProcessData(decryptedData.data(), &ciphertext[CryptoPP::AES::BLOCKSIZE], ciphertextSize);
}
