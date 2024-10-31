﻿#include <iostream>

#include "DHServer.hpp"
#include "IntegerUtils.hpp"
#include "WinSockUtils.hpp"

int main() {
    WinSockUtils::initializeWinSock();

    try {
        DHServer dhServer;
        std::cout << dhServer.startListening(3011) << '\n';
        const auto key = dhServer.exchangeKeys();
        std::cout << IntegerUtils::toHexString(key) << '\n';
    } catch (std::runtime_error& e) {
        std::cout << e.what() << '\n';
    }

    std::this_thread::sleep_for(std::chrono::seconds(10000));
    WinSockUtils::cleanupWinSock();
}
