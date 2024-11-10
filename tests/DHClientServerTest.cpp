#include "utils/dh/DHResponder.hpp"
#include "utils/dh/DHInitiator.hpp"
#include "gtest/gtest.h"
#include <integer.h>

#include "inc/utils.hpp"
#include "utils/sock/tcp/TCPUtils.hpp"

TEST_F(NetworkTestBase, KeyExchangeTest) {
    CryptoPP::Integer serverSharedSecret;
    std::thread serverThread([&] {
        const Socket socket = TCPUtils::acceptSingleConnection(PORT);
        serverSharedSecret = DHResponder::exchangeKeys(socket);
    });
    const Socket socket = TCPUtils::connectToServer(IP, PORT);
    const CryptoPP::Integer clientSharedSecret = DHInitiator::exchangeKeys(socket);
    
    serverThread.join();
    EXPECT_EQ(serverSharedSecret, clientSharedSecret);
}
