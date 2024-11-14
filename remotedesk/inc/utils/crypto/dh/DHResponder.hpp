#pragma once

#include <integer.h>

#include "../../../sock/Socket.hpp"

class DHResponder {
public:
    DHResponder() = delete;
    
    static CryptoPP::Integer exchangeKeys(const Socket& sock);
};
