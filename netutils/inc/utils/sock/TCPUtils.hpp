#pragma once

#include <string>
#include "../../sock/Socket.hpp"

class TCPUtils {
public:
    TCPUtils() = delete;

    [[nodiscard]] static Socket connectToServer(const std::string& ip, const u_short port);
    [[nodiscard]] static Socket acceptSingleConnection(const u_short port);
};
