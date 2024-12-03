#include "../../../inc/crypto/dh/DHInitiator.hpp"

#include "../../../inc/sock/dh/DHKeyExchanger.hpp"
#include "../../../inc/utils/crypto/IntegerUtils.hpp"

std::vector<byte> DHInitiator::exchangeKeys(const TCPConnection& tcpConnection) const {
    DHKeyExchanger keyExchanger(tcpConnection);
    keyExchanger.generateAndSendGroupParameters();
    return IntegerUtils::toVector(keyExchanger.exchangeKeys());
}
