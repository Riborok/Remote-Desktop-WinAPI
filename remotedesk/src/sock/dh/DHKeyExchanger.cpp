#include "../../../inc/sock/dh/DHKeyExchanger.hpp"

#include "../../../inc/utils/sock/tcp/crypto/TCPIntegerUtils.hpp"

DHKeyExchanger::DHKeyExchanger(const TCPConnection& tcpConnection): _tcpConnection(tcpConnection) {}

void DHKeyExchanger::generateAndSendGroupParameters() {
    generateRandomGroupParameters();
    sendGroupParameters();
}

void DHKeyExchanger::generateRandomGroupParameters() {
    _dhHelper.generateRandomGroupParameters();
}

void DHKeyExchanger::sendGroupParameters() const {
    TCPIntegerUtils::sendIntegers(_tcpConnection, _dhHelper.getP(), _dhHelper.getG());
}

void DHKeyExchanger::receiveGroupParameters() {
    const auto [p, g] = TCPIntegerUtils::receiveIntegers(_tcpConnection);
    _dhHelper.initializeGroupParameters(p, g);
}

CryptoPP::Integer DHKeyExchanger::exchangeKeys() {
    generateKeys();
    sendPublicKey();
    const CryptoPP::Integer serverPublicKey = receivePublicKey();
    return _dhHelper.calcSharedSecret(_privateKey, serverPublicKey);
}

void DHKeyExchanger::generateKeys() {
    _privateKey = _dhHelper.generatePrivateKey();
    _publicKey = _dhHelper.generatePublicKey(_privateKey);
}

void DHKeyExchanger::sendPublicKey() {
    TCPIntegerUtils::sendInteger(_tcpConnection, {_publicKey.data(), _publicKey.size()});
}

CryptoPP::Integer DHKeyExchanger::receivePublicKey() const {
    return TCPIntegerUtils::receiveInteger(_tcpConnection);
}
