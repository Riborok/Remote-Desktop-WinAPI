#include "../../../../inc/utils/sock/tcp/TCPUtils.hpp"

#include "../../../../inc/utils/sock/SockaddrUtils.hpp"

Socket TCPUtils::connectToServer(const std::string& ip, const u_short port) {
    Socket sock(SOCK_STREAM, IPPROTO_TCP);
    const sockaddr_in serverAddr = SockaddrUtils::initializeAddress(ip.c_str(), port);
    sock.connectToServer(serverAddr);
    return sock;
}

Socket TCPUtils::acceptSingleConnection(const u_short port) {
    const Socket listenSock(SOCK_STREAM, IPPROTO_TCP);
    listenSock.bindSocket(port);
    listenSock.listenOnSocket(1);
    Socket clientSock = listenSock.acceptConnection();
    return clientSock;
}
