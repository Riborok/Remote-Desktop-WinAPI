#pragma once
#include <winsock2.h>
#include <secblock.h>

#include "DiffieHellmanHelper.hpp"

class DHServer {
    SOCKET _listenSock, _clientSock;
    DiffieHellmanHelper _dhHelper;
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

    void bindSocket(const u_short port) const;
    void listenForClient() const;

    static void checkBindError(const int result);
    static void checkListenError(const int result);
};
