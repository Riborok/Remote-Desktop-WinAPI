#pragma once
#include <winsock2.h>
#include <secblock.h>

#include "DHHelper.hpp"

class DHServer {
    SOCKET _listenSock, _clientSock;
    DHHelper _dhHelper;
    CryptoPP::SecByteBlock _privateKey;
    CryptoPP::SecByteBlock _publicKey;
public:
    DHServer();
    bool startListening(const u_short port);
    CryptoPP::Integer exchangeKeys();
    ~DHServer();

    DHServer(const DHServer&) = delete;
    DHServer& operator=(const DHServer&) = delete;
    DHServer(DHServer&&) = delete;
    DHServer& operator=(DHServer&&) = delete;
private:
    void generateRandomGroupParameters();
    void sendGroupParameters() const;
    void generateKeys();
    void sendPublicKey();
    CryptoPP::Integer receivePublicKey() const;

    void listenForClient() const;
};
