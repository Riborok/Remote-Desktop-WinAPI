#pragma once

#include "../KeyExchanger.hpp"

class DHResponder final : public KeyExchanger {
public:
    std::vector<byte> exchangeKeys(TCPConnection& tcpConnection) override;
};
