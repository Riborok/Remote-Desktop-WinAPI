#include "../../../inc/crypto/aes/AESKeyController.hpp"

void AESKeyController::setKeyWithIV(CryptoPP::SimpleKeyingInterface& ski, const CryptoPP::SecByteBlock& iv) const {
    ski.SetKeyWithIV(_key.data(), _key.size(), iv.data());
}

AESKeyController::AESKeyController(const std::vector<byte>& key): _key(key) {}

void AESKeyController::setKey(const std::vector<byte>& key) {
    _key = key;
}
