﻿#include "crypto/dh/DHInitiator.hpp"
#include "crypto/dh/DHResponder.hpp"
#include "gtest/gtest.h"

#include "inc/utils.hpp"
#include "sock/tcp/TCPServer.hpp"
#include "utils/sock/SockaddrUtils.hpp"

TEST_F(NetworkTestBase, KeyExchangeTest) {
    std::vector<byte> serverSharedSecret;
    std::thread serverThread([&] {
        const TCPServer server(PORT);
        TCPConnection tcpConnection = server.acceptConnection();
        serverSharedSecret = DHResponder().exchangeKeys(tcpConnection);
    });
    TCPConnection tcpConnection(SockaddrUtils::createAddr(IP, PORT));
    const std::vector<byte> clientSharedSecret = DHInitiator().exchangeKeys(tcpConnection);
    
    serverThread.join();
    EXPECT_EQ(serverSharedSecret, clientSharedSecret);
}
