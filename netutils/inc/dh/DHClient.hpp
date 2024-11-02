#pragma once
#include <winsock2.h>
#include <secblock.h>

#include "DHHelper.hpp"

class DHClient {
    SOCKET _sock;
    DHHelper _dhHelper;
    CryptoPP::SecByteBlock _privateKey;
    CryptoPP::SecByteBlock _publicKey;
public:
    DHClient();
    bool connectToServer(const std::string& ip, const u_short port) const;
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
