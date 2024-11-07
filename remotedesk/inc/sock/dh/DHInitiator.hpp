#pragma once

#include <integer.h>
#include "../Socket.hpp"

class DHInitiator {
public:
    DHInitiator() = delete;
    
    static CryptoPP::Integer exchangeKeys(const Socket& sock);
};
