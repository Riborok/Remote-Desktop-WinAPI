#include "../../inc/aes/AESDecryptor.hpp"

AESDecryptor::AESDecryptor(const std::vector<byte>& key): _key(key) {
    checkKeySize();
}

std::vector<byte> AESDecryptor::decrypt(const std::vector<byte>& ciphertext) const {
    validateCiphertextLength(ciphertext);
    std::vector<byte> decrypted;
    CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption decryption;

    const CryptoPP::SecByteBlock iv = extractIV(ciphertext);
    setKeyAndIV(decryption, iv);
    performDecryption(decryption, ciphertext, decrypted);

    return decrypted;
}

void AESDecryptor::checkKeySize() const {
    if (_key.size() != CryptoPP::AES::MAX_KEYLENGTH) {
        throw std::invalid_argument("Key must be "
            + std::to_string(CryptoPP::AES::MAX_KEYLENGTH * 8) + " bits");
    }
}

void AESDecryptor::validateCiphertextLength(const std::vector<byte>& ciphertext) {
    if (ciphertext.size() < CryptoPP::AES::BLOCKSIZE) {
        throw std::invalid_argument("Ciphertext too short");
    }
}

CryptoPP::SecByteBlock AESDecryptor::extractIV(const std::vector<byte>& ciphertext) {
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    std::copy_n(ciphertext.begin(), iv.size(), iv.begin());
    return iv;
}

void AESDecryptor::setKeyAndIV(CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption& decryption,
        const CryptoPP::SecByteBlock& iv) const {
    decryption.SetKeyWithIV(_key.data(), _key.size(), iv.data());
}

void AESDecryptor::performDecryption(CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption& decryption,
        const std::vector<byte>& ciphertext, std::vector<byte>& decrypted) {
    const byte* ciphertextStart = ciphertext.data() + CryptoPP::AES::BLOCKSIZE;
    const size_t ciphertextSize = ciphertext.size() - CryptoPP::AES::BLOCKSIZE; 
    CryptoPP::StringSource stringSource(ciphertextStart, ciphertextSize, true,
        new CryptoPP::StreamTransformationFilter(decryption,
            new CryptoPP::VectorSink(decrypted))
    );
}
