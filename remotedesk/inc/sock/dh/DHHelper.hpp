﻿#pragma once

#include <integer.h>
#include <osrng.h>
#include <dh.h>
#include <secblock.h>

class DHHelper {
    CryptoPP::DH _dh;
    CryptoPP::AutoSeededRandomPool _rng;
public:
    static constexpr size_t KEY_SIZE = 32;
    
    void generateRandomGroupParameters();
    void initializeGroupParameters(const CryptoPP::Integer& p, const CryptoPP::Integer& g);
    const CryptoPP::Integer& getP() const;
    const CryptoPP::Integer& getG() const;

    CryptoPP::SecByteBlock generatePrivateKey();
    CryptoPP::SecByteBlock generatePublicKey(const CryptoPP::SecByteBlock& privKey);
    CryptoPP::Integer calcSharedSecret(const CryptoPP::SecByteBlock& privateKey, const CryptoPP::Integer& publicKey) const;
private:
    static void checkSharedSecretAgreement(const bool result);
};
