#include "../../inc/remote-desk/ParameterCommunicator.hpp"

#include "../../inc/utils/array/ByteArrayUtils.hpp"

ParameterCommunicator::ParameterCommunicator(TCPConnection& connection): _connection(connection) {}

void ParameterCommunicator::sendSize(const SIZE& size) const {
    std::vector<byte> sizeBuffer(sizeof(int) * 2);
    ByteArrayUtils::setValue<int>(sizeBuffer, 0, size.cx);
    ByteArrayUtils::setValue<int>(sizeBuffer, 1, size.cy);
    _connection.sendData(sizeBuffer);
}

SIZE ParameterCommunicator::receiveSize() const {
    int bufferSize = 2 * sizeof(int);
    const std::vector<byte> sizeBuffer = _connection.receiveData(bufferSize);
    const int width = ByteArrayUtils::getValue<int>(sizeBuffer, 0);
    const int height = ByteArrayUtils::getValue<int>(sizeBuffer, 1);
    return {width, height};
}

void ParameterCommunicator::sendUDPPort(const u_short udpPort) const {
    std::vector<byte> sizeBuffer(sizeof(u_short));
    ByteArrayUtils::setValue<u_short>(sizeBuffer, 0, udpPort);
    _connection.sendData(sizeBuffer);
}

u_short ParameterCommunicator::receiveUDPPort() const {
    int bufferSize = sizeof(u_short);
    const std::vector<byte> sizeBuffer = _connection.receiveData(bufferSize);
    return ByteArrayUtils::getValue<u_short>(sizeBuffer, 0);
}
