#pragma once

#include "TCPConnection.hpp"
#include "../../crypto/KeyExchanger.hpp"
#include "../../crypto/aes/AESDecryptor.hpp"
#include "../../crypto/aes/AESEncryptor.hpp"

class SecureTCPConnection {
    TCPConnection _connection;
    AESEncryptor _encryptor;
    AESDecryptor _decryptor;
public:
    SecureTCPConnection(const KeyExchanger& keyExchanger, TCPConnection &&connection);
    int sendData(const std::vector<byte>& buffer);
    int receiveData(std::vector<byte>& buffer);
    void setReceiveTimeout(const DWORD milliseconds) const;
    void setSendTimeout(const DWORD milliseconds) const;
};
