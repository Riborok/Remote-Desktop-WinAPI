#pragma once

#include <integer.h>
#include <osrng.h>
#include <dh.h>
#include <secblock.h>

class DiffieHellmanHelper {
    CryptoPP::DH _dh;
    CryptoPP::AutoSeededRandomPool _rng;
public:
    static constexpr int KEY_SIZE = 8 * 32;
    
    void generateRandomGroupParameters();
    void initializeGroupParameters(const CryptoPP::Integer& p, const CryptoPP::Integer& g);
    const CryptoPP::Integer& getP() const;
    const CryptoPP::Integer& getG() const;

    CryptoPP::SecByteBlock generatePrivateKey();
    CryptoPP::SecByteBlock generatePublicKey(const CryptoPP::SecByteBlock& privKey);
    CryptoPP::Integer calculateSharedSecret(const CryptoPP::SecByteBlock& privateKey, const CryptoPP::Integer& publicKey) const;
private:
    static void checkSharedSecretAgreement(const bool result);
};