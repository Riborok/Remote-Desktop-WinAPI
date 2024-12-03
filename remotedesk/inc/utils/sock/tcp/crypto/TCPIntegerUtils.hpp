#pragma once

#include <integer.h>

#include "../../../../sock/tcp/TCPConnection.hpp"

class TCPIntegerUtils {
public:
    TCPIntegerUtils() = delete;

    static void sendInteger(const TCPConnection& tcpConnection, const CryptoPP::Integer& value);
    static CryptoPP::Integer receiveInteger(const TCPConnection& tcpConnection);
};
