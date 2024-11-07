// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32
#include "../../../inc/sock/dh/DHResponder.hpp"

#include "../../../inc/sock/dh/DHKeyExchange.hpp"

CryptoPP::Integer DHResponder::exchangeKeys(Socket& sock) {
    DHKeyExchange dhKeyExchange(sock);
    dhKeyExchange.receiveGroupParameters();
    return dhKeyExchange.exchangeKeys();
}
