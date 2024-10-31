// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32
#include "../inc/DHServer.hpp"

#include "../inc/NetworkUtils.hpp"

DHServer::DHServer(): _listenSock(NetworkUtils::createSocket(SOCK_STREAM, IPPROTO_TCP)),
                      _clientSock(INVALID_SOCKET) { }

bool DHServer::startListening(const u_short port) {
    bindSocket(port);
    listenForClient();
    _clientSock = accept(_listenSock, nullptr, nullptr);
    return _clientSock != INVALID_SOCKET;
}

void DHServer::bindSocket(const u_short port) const {
    sockaddr_in serverAddr = NetworkUtils::initializeAddress(nullptr, port);
    const int result = bind(_listenSock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    checkBindError(result);
}

void DHServer::checkBindError(const int result) {
    if (result < 0) {
        throw std::runtime_error("Failed to bind socket");
    }
}

void DHServer::listenForClient() const {
    const int result = listen(_listenSock, 1);
    checkListenError(result);
}

void DHServer::checkListenError(const int result) {
    if (result < 0) {
        throw std::runtime_error("Failed to listen on socket");
    }
}

CryptoPP::Integer DHServer::exchangeKeys() {
    generateRandomGroupParameters();
    sendGroupParameters();
    generateKeys();
        
    const CryptoPP::Integer clientPublicKey = receivePublicKey();
    sendPublicKey();
    return _dhHelper.calculateSharedSecret(_privateKey, clientPublicKey);
}

void DHServer::generateRandomGroupParameters() {
    _dhHelper.generateRandomGroupParameters();
}

void DHServer::sendGroupParameters() const {
    NetworkUtils::sendInteger(_clientSock, _dhHelper.getP());
    NetworkUtils::sendInteger(_clientSock, _dhHelper.getG());
}

void DHServer::generateKeys() {
    _privateKey = _dhHelper.generatePrivateKey();
    _publicKey = _dhHelper.generatePublicKey(_privateKey);
}

void DHServer::sendPublicKey() {
    send(_clientSock, reinterpret_cast<char*>(_publicKey.BytePtr()), _publicKey.size(), 0);
}

CryptoPP::Integer DHServer::receivePublicKey() const {
    return NetworkUtils::receiveInteger(_clientSock);
}

DHServer::~DHServer() {
    closesocket(_clientSock);
    closesocket(_listenSock);
}