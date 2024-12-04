#include "../../../inc/sock/tcp/TCPServer.hpp"

TCPServer::TCPServer(const u_short port)
        : _listenSocket(SOCK_STREAM, IPPROTO_TCP) {
    _listenSocket.bindSocket(port);
    _listenSocket.listenOnSocket(SOMAXCONN);
}

TCPConnection TCPServer::acceptConnection(sockaddr_in* clientAddr, int* clientAddrSize) const {
    return TCPConnection(_listenSocket.acceptConnection(clientAddr, clientAddrSize));
}
