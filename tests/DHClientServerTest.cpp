#include "sock/dh/DHResponder.hpp"
#include "sock/dh/DHInitiator.hpp"
#include "gtest/gtest.h"
#include <integer.h>

#include "inc/utils.hpp"
#include "utils/sock/tcp/TCPUtils.hpp"

TEST_F(NetworkTestBase, KeyExchangeTest) {
    CryptoPP::Integer serverSharedSecret;
    std::thread serverThread([&] {
        Socket socket = TCPUtils::acceptSingleConnection(PORT);
        serverSharedSecret = DHResponder::exchangeKeys(socket);
    });
    Socket socket = TCPUtils::connectToServer(IP, PORT);
    const CryptoPP::Integer clientSharedSecret = DHInitiator::exchangeKeys(socket);
    
    serverThread.join();
    EXPECT_EQ(serverSharedSecret, clientSharedSecret);
}
