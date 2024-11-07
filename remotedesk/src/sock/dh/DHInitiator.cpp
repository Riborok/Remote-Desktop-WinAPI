// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32
#include "../../../inc/sock/dh/DHInitiator.hpp"

#include "../../../inc/sock/dh/DHKeyExchange.hpp"

CryptoPP::Integer DHInitiator::exchangeKeys(Socket& sock) {
    DHKeyExchange dhKeyExchange(sock);
    dhKeyExchange.generateAndSendGroupParameters();
    return dhKeyExchange.exchangeKeys();
}
