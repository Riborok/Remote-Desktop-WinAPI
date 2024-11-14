#pragma once

#include <integer.h>

#include "../../../sock/Socket.hpp"

class DHInitiator {
public:
    DHInitiator() = delete;
    
    static CryptoPP::Integer exchangeKeys(const Socket& sock);
};
