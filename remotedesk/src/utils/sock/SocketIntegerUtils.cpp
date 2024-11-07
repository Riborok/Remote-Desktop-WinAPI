#include "../../../inc/utils/sock/SocketIntegerUtils.hpp"

#include "../../../inc/sock/dh/DHHelper.hpp"
#include "../../../inc/utils/IntegerUtils.hpp"

void SocketIntegerUtils::sendInteger(const Socket& sock, const CryptoPP::Integer& value) {
    const std::vector<byte> buffer(IntegerUtils::toVector(value));
    sock.sendSocket(buffer);
}

CryptoPP::Integer SocketIntegerUtils::receiveInteger(const Socket& sock) {
    std::vector<byte> buffer(DHHelper::KEY_SIZE);
    const int len = sock.recvSocket(buffer);
    return {buffer.data(), static_cast<size_t>(len)};
}