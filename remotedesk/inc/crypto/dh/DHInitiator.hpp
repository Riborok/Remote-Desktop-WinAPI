#pragma once

#include "../KeyExchanger.hpp"

class DHInitiator final : public KeyExchanger {
public:
    std::vector<byte> exchangeKeys(TCPConnection& tcpConnection) override;
};
