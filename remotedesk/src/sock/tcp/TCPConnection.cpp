#include "../../../inc/sock/tcp/TCPConnection.hpp"

#include "../../../inc/utils/sock/SockaddrUtils.hpp"

TCPConnection::TCPConnection(Socket&& socket): _socket(std::move(socket)) {}

TCPConnection::TCPConnection(const sockaddr_in& addr)
        : _socket(SOCK_STREAM, IPPROTO_TCP){
    _socket.connectToServer(addr);
}

int TCPConnection::sendData(const std::vector<byte>& buffer) {
    return _socket.sendSocket(buffer);
}

std::vector<byte> TCPConnection::receiveData(int& bufferSize) {
    std::vector<byte> buffer(bufferSize);
    bufferSize = _socket.recvSocket(buffer);
    return buffer;
}

void TCPConnection::setReceiveTimeout(const DWORD milliseconds) const {
    _socket.setReceiveTimeout(milliseconds);
}

void TCPConnection::setSendTimeout(const DWORD milliseconds) const {
    _socket.setSendTimeout(milliseconds);
}

void TCPConnection::setSendBufferSize(const DWORD bufferSize) const {
    _socket.setSendBufferSize(bufferSize);
}

void TCPConnection::setReceiveBufferSize(const DWORD bufferSize) const {
    _socket.setReceiveBufferSize(bufferSize);
}

sockaddr_in TCPConnection::getPeerAddress() const {
    return _socket.getPeerAddress();
}

void TCPConnection::shutdownSocket(const int how) const {
    return _socket.shutdownSocket(how);
}
