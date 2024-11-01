#include "dh/DHClient.hpp"
#include "dh/DHServer.hpp"
#include "utils/WinSockUtils.hpp"
#include "gtest/gtest.h"
#include <integer.h>

TEST(DHClientServerTest, KeyExchangeTest) {
    WinSockUtils::initializeWinSock();
    
    constexpr u_short port = 3030;
    DHServer server;
    DHClient client;
    CryptoPP::Integer serverSharedSecret, clientSharedSecret;

    std::thread serverThread([&] {
        ASSERT_TRUE(server.startListening(port));
        serverSharedSecret = server.exchangeKeys();
    });

    ASSERT_TRUE(client.connectToServer("127.0.0.1", port));
    clientSharedSecret = client.exchangeKeys();

    serverThread.join();

    EXPECT_EQ(serverSharedSecret, clientSharedSecret);
    WinSockUtils::cleanupWinSock();
}
