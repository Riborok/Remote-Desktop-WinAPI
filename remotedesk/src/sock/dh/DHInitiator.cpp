#include "../../../inc/sock/dh/DHInitiator.hpp"

#include "../../../inc/sock/dh/DHKeyExchange.hpp"

CryptoPP::Integer DHInitiator::exchangeKeys(const Socket& sock) {
    DHKeyExchange dhKeyExchange(sock);
    dhKeyExchange.generateAndSendGroupParameters();
    return dhKeyExchange.exchangeKeys();
}
