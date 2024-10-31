#include "../inc/DiffieHellmanHelper.hpp"

#include "../inc/IntegerUtils.hpp"

void DiffieHellmanHelper::generateRandomGroupParameters() {
    _dh.AccessGroupParameters().GenerateRandomWithKeySize(_rng, KEY_SIZE);
}

void DiffieHellmanHelper::initializeGroupParameters(const CryptoPP::Integer& p, const CryptoPP::Integer& g) {
    _dh.AccessGroupParameters().Initialize(p, g);
}

const CryptoPP::Integer& DiffieHellmanHelper::getP() const { return _dh.GetGroupParameters().GetModulus(); }

const CryptoPP::Integer& DiffieHellmanHelper::getG() const { return _dh.GetGroupParameters().GetGenerator(); }

CryptoPP::SecByteBlock DiffieHellmanHelper::generatePrivateKey() {
    CryptoPP::SecByteBlock privKey(_dh.PrivateKeyLength());
    _dh.GeneratePrivateKey(_rng, privKey);
    return privKey;
}

CryptoPP::SecByteBlock DiffieHellmanHelper::generatePublicKey(const CryptoPP::SecByteBlock& privKey) {
    CryptoPP::SecByteBlock pubKey(_dh.PublicKeyLength());
    _dh.GeneratePublicKey(_rng, privKey, pubKey);
    return pubKey;
}

CryptoPP::Integer DiffieHellmanHelper::calculateSharedSecret(const CryptoPP::SecByteBlock& privateKey, const CryptoPP::Integer& publicKey) const {
    CryptoPP::SecByteBlock sharedSecret(_dh.AgreedValueLength());
    CryptoPP::SecByteBlock encodedPublicKey(IntegerUtils::toSecByteBlock(publicKey));
    const bool result = _dh.Agree(sharedSecret, privateKey, encodedPublicKey);
    checkSharedSecretAgreement(result);
    return {sharedSecret, sharedSecret.size()};
}

void DiffieHellmanHelper::checkSharedSecretAgreement(const bool result) {
    if (!result) {
        throw std::runtime_error("Failed to compute shared secret");
    }
}