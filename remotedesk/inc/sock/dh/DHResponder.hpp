#pragma once

#include <integer.h>
#include "../Socket.hpp"

class DHResponder {
public:
    DHResponder() = delete;
    
    static CryptoPP::Integer exchangeKeys(const Socket& sock);
};
