#include "../../../../../inc/utils/sock/tcp/crypto/TCPIntegerUtils.hpp"

#include "../../../../../inc/sock/dh/DHHelper.hpp"
#include "../../../../../inc/utils/array/ByteArrayUtils.hpp"
#include "../../../../../inc/utils/crypto/IntegerUtils.hpp"

void TCPIntegerUtils::sendInteger(TCPConnection& tcpConnection, const CryptoPP::Integer& value) {
    const std::vector<byte> buffer(IntegerUtils::toVector(value));
    tcpConnection.sendData(buffer);
}

CryptoPP::Integer TCPIntegerUtils::receiveInteger(TCPConnection& tcpConnection) {
    int bufferSize = DHHelper::KEY_SIZE;
    std::vector<byte> buffer = tcpConnection.receiveData(bufferSize);
    return {buffer.data(), static_cast<size_t>(bufferSize)};
}

void TCPIntegerUtils::sendIntegers(TCPConnection& tcpConnection, const CryptoPP::Integer& p,
        const CryptoPP::Integer& g) {
    const std::vector<byte> bufferP = IntegerUtils::toVector(p);
    const std::vector<byte> bufferG = IntegerUtils::toVector(g);
    const std::vector<byte> combinedBuffer = combineBuffers(bufferP, bufferG);
    tcpConnection.sendData(combinedBuffer);
}

std::vector<byte> TCPIntegerUtils::combineBuffers(const std::vector<byte>& bufferP, const std::vector<byte>& bufferG) {
    const size_t sizeP = bufferP.size();
    const size_t sizeG = bufferG.size();
    std::vector<byte> combinedBuffer(2 * sizeof(size_t) + sizeP + sizeG);
    ByteArrayUtils::setValueByByteIndex<size_t>(combinedBuffer, 0, sizeP);
    memcpy(&combinedBuffer[sizeof(size_t)], bufferP.data(), sizeP);
    ByteArrayUtils::setValueByByteIndex<size_t>(combinedBuffer, sizeof(size_t) + sizeP, sizeG);
    memcpy(&combinedBuffer[2 * sizeof(size_t) + sizeP], bufferG.data(), sizeG);
    return combinedBuffer;
}

std::pair<CryptoPP::Integer, CryptoPP::Integer> TCPIntegerUtils::receiveIntegers(TCPConnection& tcpConnection) {
    const std::vector<byte> combinedBuffer = receiveDataFromConnection(tcpConnection);
    auto [bufferP, bufferG] = extractIntegerBuffers(combinedBuffer);
    CryptoPP::Integer p(bufferP.data(), bufferP.size());
    CryptoPP::Integer g(bufferG.data(), bufferG.size());
    return {p, g};
}

std::vector<byte> TCPIntegerUtils::receiveDataFromConnection(TCPConnection& tcpConnection) {
    int bufferSize = 2 * sizeof(size_t) + 2 * DHHelper::KEY_SIZE;
    std::vector<byte> combinedBuffer = tcpConnection.receiveData(bufferSize);
    if (bufferSize < static_cast<int>(2 * sizeof(size_t))) {
        throw std::runtime_error("Received incomplete data for integers");
    }
    return combinedBuffer;
}

std::pair<std::vector<byte>, std::vector<byte>> TCPIntegerUtils::extractIntegerBuffers(const std::vector<byte>& combinedBuffer) {
    const size_t sizeP = ByteArrayUtils::getValueByByteIndex<size_t>(combinedBuffer, 0);
    std::vector<byte> bufferP(sizeP);
    memcpy(bufferP.data(), &combinedBuffer[sizeof(size_t)], sizeP);
    const size_t sizeG = ByteArrayUtils::getValueByByteIndex<size_t>(combinedBuffer, sizeof(size_t) + sizeP);
    std::vector<byte> bufferG(sizeG);
    memcpy(bufferG.data(), &combinedBuffer[2 * sizeof(size_t) + sizeP], sizeG);
    return {bufferP, bufferG};
}
