#pragma once

#include "DHHelper.hpp"
#include "../Socket.hpp"

class DHKeyExchange {
    DHHelper _dhHelper;
    CryptoPP::SecByteBlock _privateKey;
    CryptoPP::SecByteBlock _publicKey;
    Socket& _sock;
public:
    explicit DHKeyExchange(Socket& socket);
    void generateAndSendGroupParameters();
    void receiveGroupParameters();
    CryptoPP::Integer exchangeKeys();
private:
    void generateRandomGroupParameters();
    void sendGroupParameters() const;
    void generateKeys();
    void sendPublicKey();
    CryptoPP::Integer receivePublicKey() const;
};
