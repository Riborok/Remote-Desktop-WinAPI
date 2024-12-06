#pragma once

#include "TCPConnection.hpp"
#include "../../crypto/aes/AESDecryptor.hpp"
#include "../../crypto/aes/AESEncryptor.hpp"

class SecureTCPConnection final : public TCPConnection {
    AESEncryptor _encryptor;
    AESDecryptor _decryptor;
public:
    SecureTCPConnection(const std::vector<byte>& key, TCPConnection &&connection);
    int sendData(const std::vector<byte>& buffer) override;
    std::vector<byte> receiveData(int& bufferSize) override;
private:
    static void adjustBufferSizeForMetadata(int& bufferSize);
    static bool isBufferSizeSufficient(const int bufferSize);
    std::vector<byte> createDecryptedData(const std::vector<byte>& buffer, int& bufferSize);
    static std::vector<byte> createEmptyData(int& bufferSize);
};
