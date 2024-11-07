#include "../../../inc/sock/dh/DHKeyExchange.hpp"

#include "../../../inc/utils/sock/SocketIntegerUtils.hpp"

DHKeyExchange::DHKeyExchange(const Socket& socket): _sock(socket) {}

void DHKeyExchange::generateAndSendGroupParameters() {
    generateRandomGroupParameters();
    sendGroupParameters();
}

void DHKeyExchange::generateRandomGroupParameters() {
    _dhHelper.generateRandomGroupParameters();
}

void DHKeyExchange::sendGroupParameters() const {
    SocketIntegerUtils::sendInteger(_sock, _dhHelper.getP());
    SocketIntegerUtils::sendInteger(_sock, _dhHelper.getG());
}

void DHKeyExchange::receiveGroupParameters() {
    const CryptoPP::Integer p = SocketIntegerUtils::receiveInteger(_sock);
    const CryptoPP::Integer g = SocketIntegerUtils::receiveInteger(_sock);
    _dhHelper.initializeGroupParameters(p, g);
}

CryptoPP::Integer DHKeyExchange::exchangeKeys() {
    generateKeys();
    sendPublicKey();
    const CryptoPP::Integer serverPublicKey = receivePublicKey();
    return _dhHelper.calculateSharedSecret(_privateKey, serverPublicKey);
}

void DHKeyExchange::generateKeys() {
    _privateKey = _dhHelper.generatePrivateKey();
    _publicKey = _dhHelper.generatePublicKey(_privateKey);
}

void DHKeyExchange::sendPublicKey() {
    SocketIntegerUtils::sendInteger(_sock, {_publicKey.data(), _publicKey.size()});
}

CryptoPP::Integer DHKeyExchange::receivePublicKey() const {
    return SocketIntegerUtils::receiveInteger(_sock);
}
