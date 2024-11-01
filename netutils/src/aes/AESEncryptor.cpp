#include "../../inc/aes/AESEncryptor.hpp"

AESEncryptor::AESEncryptor(const std::vector<byte>& key): _key(key) {
    checkKeySize();
}

std::vector<byte> AESEncryptor::encrypt(const std::vector<byte>& data) const {
    std::vector<byte> ciphertext;
    CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption encryption;
        
    const CryptoPP::SecByteBlock iv = generateIV();
    appendIVToCiphertext(iv, ciphertext);
    setKeyAndIV(encryption, iv);
    performEncryption(encryption, data, ciphertext);

    return ciphertext;
}

void AESEncryptor::checkKeySize() const {
    if (_key.size() != CryptoPP::AES::MAX_KEYLENGTH) {
        throw std::invalid_argument("Key must be "
            + std::to_string(CryptoPP::AES::MAX_KEYLENGTH * 8) + " bits");
    }
}

CryptoPP::SecByteBlock AESEncryptor::generateIV() {
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    CryptoPP::AutoSeededRandomPool rng;
    rng.GenerateBlock(iv, iv.size());
    return iv;
}

void AESEncryptor::appendIVToCiphertext(const CryptoPP::SecByteBlock& iv, std::vector<byte>& ciphertext) {
    ciphertext.insert(ciphertext.end(), iv.begin(), iv.end());
}

void AESEncryptor::setKeyAndIV(CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption& encryption,
        const CryptoPP::SecByteBlock& iv) const {
    encryption.SetKeyWithIV(_key.data(), _key.size(), iv.data());
}

void AESEncryptor::performEncryption(CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption& encryption,
        const std::vector<byte>& data, std::vector<byte>& ciphertext) {
    CryptoPP::StringSource stringSource(data.data(), data.size(), true,
        new CryptoPP::StreamTransformationFilter(encryption,
            new CryptoPP::VectorSink(ciphertext))
    );
}
