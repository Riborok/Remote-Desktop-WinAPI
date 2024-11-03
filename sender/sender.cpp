#include "utils/sock/WinSockUtils.hpp"

int main() {
    WinSockUtils::initializeWinSock();

    WinSockUtils::cleanupWinSock();
}
