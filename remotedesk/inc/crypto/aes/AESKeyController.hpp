#pragma once

#include <vector>
#include <aes.h>

#include "../../types/byte.hpp"

class AESKeyController {
    std::vector<byte> _key;
protected:
    void setKeyWithIV(CryptoPP::SimpleKeyingInterface& ski, const CryptoPP::SecByteBlock& iv) const;
public:
    explicit AESKeyController(const std::vector<byte>& key);
    AESKeyController() = default;
    void setKey(const std::vector<byte>& key);
    
    virtual ~AESKeyController() = default;
    AESKeyController(AESKeyController&&) = default;
    AESKeyController& operator=(AESKeyController&&) = default;
    AESKeyController(const AESKeyController&) = default;
    AESKeyController& operator=(const AESKeyController&) = default;
};
