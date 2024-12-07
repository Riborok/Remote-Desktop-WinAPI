#pragma once

#include <integer.h>

#include "../../../../sock/tcp/TCPConnection.hpp"

class TCPKeyExchangeUtils {
public:
    TCPKeyExchangeUtils() = delete;

    static void sendPublicKey(TCPConnection& tcpConnection, const CryptoPP::Integer& value);
    static CryptoPP::Integer receivePublicKey(TCPConnection& tcpConnection);
    static void sendDHParameters(TCPConnection& tcpConnection, const CryptoPP::Integer& p, const CryptoPP::Integer& g);
    static std::pair<CryptoPP::Integer, CryptoPP::Integer> receiveDHParameters(TCPConnection& tcpConnection);
private:
    static std::vector<byte> combineBuffers(const std::vector<byte>& bufferP, const std::vector<byte>& bufferG);
    static std::vector<byte> receiveBufferFromConnection(TCPConnection& tcpConnection);
    static std::pair<std::vector<byte>, std::vector<byte>> extractDHParameters(const std::vector<byte>& combinedBuffer);
};
