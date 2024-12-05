// ReSharper disable CppClangTidyClangDiagnosticSignCompare
#include "../../../inc/sock/tcp/SecureTCPConnection.hpp"

#include "../../../inc/utils/crypto/aes/AESToolkit.hpp"

SecureTCPConnection::SecureTCPConnection(const std::vector<byte>& key, TCPConnection&& connection)
        : _connection(std::move(connection)), _encryptor(key), _decryptor(key) { }

int SecureTCPConnection::sendData(const std::vector<byte>& buffer) {
    return _connection.sendData(_encryptor.encrypt(buffer));
}

int SecureTCPConnection::receiveData(std::vector<byte>& buffer) {
    const int result = _connection.receiveData(buffer);
    if (result >= AESToolkit::METADATA_SIZE) {
        buffer = _decryptor.decrypt(buffer);
    }
    return result;
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
