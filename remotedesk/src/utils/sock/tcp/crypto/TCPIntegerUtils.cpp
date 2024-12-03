#include "../../../../../inc/utils/sock/tcp/crypto/TCPIntegerUtils.hpp"

#include "../../../../../inc/sock/dh/DHHelper.hpp"
#include "../../../../../inc/utils/crypto/IntegerUtils.hpp"

void TCPIntegerUtils::sendInteger(const TCPConnection& tcpConnection, const CryptoPP::Integer& value) {
    const std::vector<byte> buffer(IntegerUtils::toVector(value));
    tcpConnection.sendData(buffer);
}

CryptoPP::Integer TCPIntegerUtils::receiveInteger(const TCPConnection& tcpConnection) {
    std::vector<byte> buffer(DHHelper::KEY_SIZE);
    const int len = tcpConnection.receiveData(buffer);
    return {buffer.data(), static_cast<size_t>(len)};
}