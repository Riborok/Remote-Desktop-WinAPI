#include "remote-desk/sender/Sender.hpp"
#include "utils/sock/WinSockUtils.hpp"

int main() {
    WinSockUtils::initializeWinSock();

    Sender sender(8080, {1920, 1080}, 30, 1000);
    sender.run();
    while (!GetAsyncKeyState(VK_ESCAPE)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    sender.stop();
    WinSockUtils::cleanupWinSock();
    return 0;
}
