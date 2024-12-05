#pragma once

#include "TCPConnection.hpp"
#include "../../crypto/aes/AESDecryptor.hpp"
#include "../../crypto/aes/AESEncryptor.hpp"

class SecureTCPConnection {
    TCPConnection _connection;
    AESEncryptor _encryptor;
    AESDecryptor _decryptor;
public:
    SecureTCPConnection(const std::vector<byte>& key, TCPConnection &&connection);
    int sendData(const std::vector<byte>& buffer);
    std::vector<byte> receiveData(int& bufferSize);
    void setReceiveTimeout(const DWORD milliseconds) const;
    void setSendTimeout(const DWORD milliseconds) const;
    void setSendBufferSize(const DWORD bufferSize) const;
    void setReceiveBufferSize(const DWORD bufferSize) const;
    sockaddr_in getPeerAddress() const;
};
