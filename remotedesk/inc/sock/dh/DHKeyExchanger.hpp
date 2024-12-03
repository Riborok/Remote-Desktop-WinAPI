#pragma once

#include "DHHelper.hpp"
#include "../tcp/TCPConnection.hpp"

class DHKeyExchanger {
    DHHelper _dhHelper;
    CryptoPP::SecByteBlock _privateKey;
    CryptoPP::SecByteBlock _publicKey;
    const TCPConnection& _tcpConnection;
public:
    explicit DHKeyExchanger(const TCPConnection& tcpConnection);
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
