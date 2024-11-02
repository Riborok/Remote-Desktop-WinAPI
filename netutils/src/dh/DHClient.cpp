// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32
#include "../../inc/dh/DHClient.hpp"

#include "../../inc/utils/NetworkUtils.hpp"

DHClient::DHClient(): _sock(NetworkUtils::createSocket(SOCK_STREAM, IPPROTO_TCP)) { }

bool DHClient::connectToServer(const std::string& ip, const u_short port) const {
    sockaddr_in serverAddr = NetworkUtils::initializeAddress(ip.c_str(), port);
    const int result = connect(_sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    return result >= 0;
}

CryptoPP::Integer DHClient::exchangeKeys() {
    receiveGroupParameters();
    generateKeys();
        
    sendPublicKey();
    const CryptoPP::Integer serverPublicKey = receivePublicKey();
    return _dhHelper.calculateSharedSecret(_privateKey, serverPublicKey);
}

void DHClient::receiveGroupParameters() {
    const CryptoPP::Integer p = NetworkUtils::receiveInteger(_sock);
    const CryptoPP::Integer g = NetworkUtils::receiveInteger(_sock);
    _dhHelper.initializeGroupParameters(p, g);
}

void DHClient::generateKeys() {
    _privateKey = _dhHelper.generatePrivateKey();
    _publicKey = _dhHelper.generatePublicKey(_privateKey);
}

void DHClient::sendPublicKey() {
    send(_sock, reinterpret_cast<char*>(_publicKey.BytePtr()), _publicKey.size(), 0);
}

CryptoPP::Integer DHClient::receivePublicKey() const {
    return NetworkUtils::receiveInteger(_sock);
}

DHClient::~DHClient() {
    closesocket(_sock);
}