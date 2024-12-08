#include "../../../inc/utils/remote-desk/SenderInitializer.hpp"

#include "../../../inc/crypto/dh/DHInitiator.hpp"
#include "../../../inc/remote-desk/ParameterCommunicator.hpp"
#include "../../../inc/sock/tcp/SecureTCPConnection.hpp"
#include "../../../inc/sock/tcp/TCPServer.hpp"
#include "../../../inc/sock/udp/data-fragmenter/ImgCodecSecureFragmenter.hpp"
#include "../../../inc/utils/sock/SockaddrUtils.hpp"

std::unique_ptr<ThreadSafeQueue<std::vector<byte>>> SenderInitializer::createFrames() {
    return std::make_unique<ThreadSafeQueue<std::vector<byte>>>();
}

std::unique_ptr<TCPConnection> SenderInitializer::createConnection(const u_short tcpServerPort) {
    const TCPServer tcpServer(tcpServerPort);
    TCPConnection tcpConnection = tcpServer.acceptConnection();
    auto key = DHInitiator().exchangeKeys(tcpConnection);
    auto secureTcpConnection = std::make_unique<SecureTCPConnection>(key, std::move(tcpConnection));
    return secureTcpConnection;
}

u_short SenderInitializer::sendSizeReceivePort(TCPConnection& connection, const SIZE& targetSize) {
    const ParameterCommunicator parameterCommunicator(connection);
    parameterCommunicator.sendSize(targetSize);
    return parameterCommunicator.receiveUDPPort();
}

std::unique_ptr<UDPSenderWorker> SenderInitializer::createUDPSenderWorker(ThreadSafeQueue<std::vector<byte>>& frames,
        TCPConnection& connection, const u_short udpPort, const SIZE& targetSize, const ImageConfig& ic) {
    auto key = DHInitiator().exchangeKeys(connection);
    auto dataFragmenter = std::make_unique<ImgCodecSecureFragmenter>(ImageTileSplitter{targetSize, ic}, key);
    const std::string ip = SockaddrUtils::getIpAddress(connection.getPeerAddress());
    return std::make_unique<UDPSenderWorker>(frames,
        UDPSender{SockaddrUtils::createAddr(ip, udpPort), std::move(dataFragmenter)});
}

std::unique_ptr<ScreenCaptureWorker> SenderInitializer::createScreenCaptureWorker(
        ThreadSafeQueue<std::vector<byte>>& frames, const SIZE& targetSize, const int fps, const int maxDelayMs) {
    return std::make_unique<ScreenCaptureWorker>(frames, targetSize, fps, maxDelayMs);
}

std::unique_ptr<RemoteEventExecutor> SenderInitializer::createRemoteEventExecutor(
        std::unique_ptr<TCPConnection>&& connection, const SIZE& targetSize) {
    return std::make_unique<RemoteEventExecutor>(std::move(connection), targetSize);
}
