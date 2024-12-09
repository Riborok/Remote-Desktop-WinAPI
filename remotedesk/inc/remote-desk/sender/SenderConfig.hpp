#pragma once

#include "../../../inc/img/ImageConfig.hpp"
#include <windows.h>

struct SenderConfig {
    u_short tcpServerPort;
    ImageConfig imageConfig;
    int fps;
    int maxDelayMs;
    SIZE targetSize;
};
