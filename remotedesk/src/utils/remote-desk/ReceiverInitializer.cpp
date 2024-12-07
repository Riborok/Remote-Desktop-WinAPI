#include "../../../inc/utils/remote-desk/ReceiverInitializer.hpp"

#include "../../../inc/crypto/dh/DHResponder.hpp"
#include "../../../inc/remote-desk/ParameterCommunicator.hpp"
#include "../../../inc/sock/tcp/SecureTCPConnection.hpp"
#include "../../../inc/sock/udp/data-reassembler/ImgCodecSecureReassembler.hpp"

std::unique_ptr<ThreadSafeQueue<std::vector<byte>>> ReceiverInitializer::createFrames() {
    return std::make_unique<ThreadSafeQueue<std::vector<byte>>>();
}

std::unique_ptr<TCPConnection> ReceiverInitializer::createConnection(const sockaddr_in& addr) {
    TCPConnection tcpConnection(addr);
    auto key = DHResponder().exchangeKeys(tcpConnection);
    auto secureTcpConnection = std::make_unique<SecureTCPConnection>(key, std::move(tcpConnection));
    return secureTcpConnection;
}

SIZE ReceiverInitializer::sendPortReceiveSize(TCPConnection& connection, const u_short udpPort) {
    const ParameterCommunicator parameterCommunicator(connection);
    parameterCommunicator.sendUDPPort(udpPort);
    return parameterCommunicator.receiveSize();
}

std::unique_ptr<UDPReceiverWorker> ReceiverInitializer::createUDPReceiverWorker(ThreadSafeQueue<std::vector<byte>>& frames,
        TCPConnection& connection, const u_short udpPort, const SIZE& receivedSize) {
    auto key = DHResponder().exchangeKeys(connection);
    std::unique_ptr<DataReassembler> dataReassembler = std::make_unique<ImgCodecSecureReassembler>(receivedSize, key);
    return std::make_unique<UDPReceiverWorker>(frames, UDPReceiver{udpPort, std::move(dataReassembler)});
}

std::unique_ptr<ScreenRenderWorker> ReceiverInitializer::createScreenRenderWorker(
        ThreadSafeQueue<std::vector<byte>>& frames, const HWND hWnd, const SIZE& receivedSize, const int fps,
    const int maxDelayMs) {
    return std::make_unique<ScreenRenderWorker>(frames, hWnd, SIZE{}, receivedSize, fps, maxDelayMs);
}

std::unique_ptr<EventSender> ReceiverInitializer::createEventSender(std::unique_ptr<TCPConnection>&& connection,
        const SIZE& receivedSize) {
    return std::make_unique<EventSender>(std::move(connection), SIZE{}, receivedSize);
}
