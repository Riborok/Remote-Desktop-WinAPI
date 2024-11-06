#pragma once

#include <string>
#include "../../sock/Socket.hpp"

class TCPUtils {
public:
    TCPUtils() = delete;

    static Socket connectToServer(const std::string& ip, const u_short port);
    static Socket acceptSingleConnection(const u_short port);
};
