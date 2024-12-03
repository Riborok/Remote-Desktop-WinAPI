// ReSharper disable CppClangTidyClangDiagnosticSignCompare
#include "../../../inc/sock/tcp/SecureTCPConnection.hpp"

#include "../../../inc/utils/crypto/aes/AESToolkit.hpp"

SecureTCPConnection::SecureTCPConnection(const KeyExchanger& keyExchanger, TCPConnection&& connection)
        : _connection(std::move(connection)) {
    const std::vector<byte> key = keyExchanger.exchangeKeys(_connection);
    _encryptor.setKey(key);
    _decryptor.setKey(key);
}

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
