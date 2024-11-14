#pragma once

#include <integer.h>
#include "../../../sock/Socket.hpp"

class SocketIntegerUtils {
public:
    SocketIntegerUtils() = delete;

    static void sendInteger(const Socket& sock, const CryptoPP::Integer& value);
    static CryptoPP::Integer receiveInteger(const Socket& sock);
};
