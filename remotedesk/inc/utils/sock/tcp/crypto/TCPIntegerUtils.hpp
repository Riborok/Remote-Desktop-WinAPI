#pragma once

#include <integer.h>

#include "../../../../sock/tcp/TCPConnection.hpp"

class TCPIntegerUtils {
public:
    TCPIntegerUtils() = delete;

    static void sendInteger(TCPConnection& tcpConnection, const CryptoPP::Integer& value);
    static CryptoPP::Integer receiveInteger(TCPConnection& tcpConnection);
    static void sendIntegers(TCPConnection& tcpConnection, const CryptoPP::Integer& p, const CryptoPP::Integer& g);
    static std::pair<CryptoPP::Integer, CryptoPP::Integer> receiveIntegers(TCPConnection& tcpConnection);
private:
    static std::vector<byte> combineBuffers(const std::vector<byte>& bufferP, const std::vector<byte>& bufferG);
    static std::vector<byte> receiveDataFromConnection(TCPConnection& tcpConnection);
    static std::pair<std::vector<byte>, std::vector<byte>> extractIntegerBuffers(const std::vector<byte>& combinedBuffer);
};
