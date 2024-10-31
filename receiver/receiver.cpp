#include <iostream>

#include "DHClient.hpp"
#include "IntegerUtils.hpp"
#include "WinSockUtils.hpp"

int main() {
    WinSockUtils::initializeWinSock();

    try {
        DHClient dhClient;
        std::cout << dhClient.connectToServer("127.0.0.1", 3011) << '\n';
        const auto key = dhClient.exchangeKeys();
        std::cout << IntegerUtils::toHexString(key) << '\n';
    } catch (std::runtime_error& e) {
        std::cout << e.what() << '\n';
    }

    std::this_thread::sleep_for(std::chrono::seconds(10000));
    WinSockUtils::cleanupWinSock();
}
