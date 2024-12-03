#include "../../../inc/sock/tcp/TCPConnection.hpp"

#include "../../../inc/utils/sock/SockaddrUtils.hpp"

TCPConnection::TCPConnection(Socket&& socket): _socket(std::move(socket)) {}

TCPConnection::TCPConnection(const std::string& ip, const u_short port)
        : _socket(SOCK_STREAM, IPPROTO_TCP){
    const sockaddr_in serverAddr = SockaddrUtils::initializeAddress(ip.c_str(), port);
    _socket.connectToServer(serverAddr);
}

int TCPConnection::sendData(const std::vector<byte>& buffer) const {
    return _socket.sendSocket(buffer);
}

int TCPConnection::receiveData(std::vector<byte>& buffer) const {
    return _socket.recvSocket(buffer);
}

void TCPConnection::setReceiveTimeout(const DWORD milliseconds) const {
    _socket.setReceiveTimeout(milliseconds);
}

void TCPConnection::setSendTimeout(const DWORD milliseconds) const {
    _socket.setSendTimeout(milliseconds);
}
