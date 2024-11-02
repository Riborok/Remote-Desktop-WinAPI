#include "dh/DHClient.hpp"
#include "dh/DHServer.hpp"
#include "gtest/gtest.h"
#include <integer.h>

#include "inc/utils.hpp"

TEST_F(NetworkTestBase, KeyExchangeTest) {
    DHServer server;
    DHClient client;
    
    CryptoPP::Integer serverSharedSecret;
    std::thread serverThread([&] {
        ASSERT_TRUE(server.startListening(PORT));
        serverSharedSecret = server.exchangeKeys();
    });

    ASSERT_TRUE(client.connectToServer(IP, PORT));
    const CryptoPP::Integer clientSharedSecret = client.exchangeKeys();

    serverThread.join();

    EXPECT_EQ(serverSharedSecret, clientSharedSecret);
}
