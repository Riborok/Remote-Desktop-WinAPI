#pragma once

#pragma comment(lib, "ws2_32.lib")

class WinSockUtils {
public:
    WinSockUtils() = delete;
    
    static void initializeWinSock();
    static void cleanupWinSock();
private:
    static void checkWinSockError(const int result);
};
