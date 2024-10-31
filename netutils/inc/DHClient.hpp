#pragma once
#include <winsock2.h>
#include <secblock.h>

#include "DiffieHellmanHelper.hpp"

class DHClient {
    SOCKET _sock;
    DiffieHellmanHelper _dhHelper;
    CryptoPP::SecByteBlock _privateKey;
    CryptoPP::SecByteBlock _publicKey;

public:
    DHClient();
    bool connectToServer(const char* ip, const u_short port) const;
    CryptoPP::Integer exchangeKeys();
    ~DHClient();

    DHClient(const DHClient&) = delete;
    DHClient& operator=(const DHClient&) = delete;
    DHClient(DHClient&&) = delete;
    DHClient& operator=(DHClient&&) = delete;
private:
    void receiveGroupParameters();
    void generateKeys();
    void sendPublicKey();
    CryptoPP::Integer receivePublicKey() const;
};
