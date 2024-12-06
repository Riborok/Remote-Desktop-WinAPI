// ReSharper disable CppClangTidyClangDiagnosticSignCompare
#include "../../../inc/sock/tcp/SecureTCPConnection.hpp"

#include "../../../inc/utils/crypto/aes/AESToolkit.hpp"

SecureTCPConnection::SecureTCPConnection(const std::vector<byte>& key, TCPConnection&& connection)
        : _connection(std::move(connection)), _encryptor(key), _decryptor(key) { }

int SecureTCPConnection::sendData(const std::vector<byte>& buffer) {
    return _connection.sendData(_encryptor.encrypt(buffer));
}

std::vector<byte> SecureTCPConnection::receiveData(int& bufferSize) {
    std::vector<byte> buffer(bufferSize + AESToolkit::METADATA_SIZE);
    bufferSize = _connection.receiveData(buffer);
    if (bufferSize >= AESToolkit::METADATA_SIZE) {
        buffer = _decryptor.decrypt(buffer);
    }
    return buffer;
}

void SecureTCPConnection::setReceiveTimeout(const DWORD milliseconds) const {
    _connection.setReceiveTimeout(milliseconds);
}

void SecureTCPConnection::setSendTimeout(const DWORD milliseconds) const {
    _connection.setSendTimeout(milliseconds);
}

void SecureTCPConnection::setSendBufferSize(const DWORD bufferSize) const {
    _connection.setSendBufferSize(bufferSize);
}

void SecureTCPConnection::setReceiveBufferSize(const DWORD bufferSize) const {
    _connection.setReceiveBufferSize(bufferSize);
}

sockaddr_in SecureTCPConnection::getPeerAddress() const {
    return _connection.getPeerAddress();
}

void SecureTCPConnection::shutdownSocket(const int how) const {
    return _connection.shutdownSocket(how);
}
