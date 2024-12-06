// ReSharper disable CppClangTidyClangDiagnosticSignCompare
#include "../../../inc/sock/tcp/SecureTCPConnection.hpp"

#include "../../../inc/utils/crypto/aes/AESToolkit.hpp"

SecureTCPConnection::SecureTCPConnection(const std::vector<byte>& key, TCPConnection&& connection)
        : TCPConnection(std::move(connection)), _encryptor(key), _decryptor(key) { }

int SecureTCPConnection::sendData(const std::vector<byte>& buffer) {
    return TCPConnection::sendData(_encryptor.encrypt(buffer));
}

std::vector<byte> SecureTCPConnection::receiveData(int& bufferSize) {
    adjustBufferSizeForMetadata(bufferSize);
    const std::vector<byte> buffer = TCPConnection::receiveData(bufferSize);
    if (isBufferSizeSufficient(bufferSize)) {
        return createDecryptedData(buffer, bufferSize);
    }
    return createEmptyData(bufferSize);
}

void SecureTCPConnection::adjustBufferSizeForMetadata(int& bufferSize) {
    bufferSize += AESToolkit::METADATA_SIZE;
}

bool SecureTCPConnection::isBufferSizeSufficient(const int bufferSize) {
    return bufferSize >= AESToolkit::METADATA_SIZE;
}

std::vector<byte> SecureTCPConnection::createDecryptedData(const std::vector<byte>& buffer, int& bufferSize) {
    bufferSize -= AESToolkit::METADATA_SIZE;
    return _decryptor.decrypt(buffer);
}

std::vector<byte> SecureTCPConnection::createEmptyData(int& bufferSize) {
    bufferSize = 0;
    return {};
}