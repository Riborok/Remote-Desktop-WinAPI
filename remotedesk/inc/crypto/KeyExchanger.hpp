﻿#pragma once

#include "../sock/tcp/TCPConnection.hpp"

class KeyExchanger {
public:
    virtual std::vector<byte> exchangeKeys(const TCPConnection& tcpConnection) const = 0;

    KeyExchanger() = default;
    virtual ~KeyExchanger() = default;
    KeyExchanger(KeyExchanger&&) = default;
    KeyExchanger& operator=(KeyExchanger&&) = default;
    KeyExchanger(const KeyExchanger&) = default;
    KeyExchanger& operator=(const KeyExchanger&) = default;
};