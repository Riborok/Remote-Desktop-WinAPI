#include "../inc/IntegerUtils.hpp"

#include <hex.h>

std::vector<byte> IntegerUtils::toVector(const CryptoPP::Integer& value) {
    std::vector<byte> buffer(value.ByteCount());
    value.Encode(buffer.data(), buffer.size());
    return buffer;
}

CryptoPP::SecByteBlock IntegerUtils::toSecByteBlock(const CryptoPP::Integer& value) {
    CryptoPP::SecByteBlock buffer(value.MinEncodedSize());
    value.Encode(buffer.BytePtr(), buffer.size());
    return buffer;
}

std::string IntegerUtils::toHexString(const CryptoPP::Integer& value) {
    std::string hexString;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(hexString));
    value.Encode(encoder, value.ByteCount());
    encoder.MessageEnd();
    return hexString;
}
