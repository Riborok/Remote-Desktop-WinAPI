#include "../../../inc/crypto/dh/DHResponder.hpp"

#include "../../../inc/sock/dh/DHKeyExchanger.hpp"
#include "../../../inc/utils/crypto/IntegerUtils.hpp"

std::vector<byte> DHResponder::exchangeKeys(const TCPConnection& tcpConnection) const {
    DHKeyExchanger keyExchanger(tcpConnection);
    keyExchanger.receiveGroupParameters();
    return IntegerUtils::toVector(keyExchanger.exchangeKeys());
}
