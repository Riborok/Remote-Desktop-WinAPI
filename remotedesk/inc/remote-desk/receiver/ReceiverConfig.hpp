#pragma once

#include <windows.h>

struct ReceiverConfig {
    ULONG serverIp;
    u_short serverPort;
    u_short udpPort;
    int fps;
    int maxDelayMs;
};
