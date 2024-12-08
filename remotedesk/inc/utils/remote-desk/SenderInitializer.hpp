#pragma once

#include <memory>

#include "../../img/ImageConfig.hpp"
#include "../../remote-desk/sender/RemoteEventExecutor.hpp"
#include "../../screen/capture/ScreenCaptureWorker.hpp"
#include "../../sock/tcp/TCPConnection.hpp"
#include "../../sock/udp/sender/UDPSenderWorker.hpp"

class SenderInitializer {
public:
    SenderInitializer() = delete;

    static std::unique_ptr<ThreadSafeQueue<std::vector<byte>>> createFrames();
    static std::unique_ptr<TCPConnection> createConnection(const u_short tcpServerPort);
    static u_short sendSizeReceivePort(TCPConnection& connection, const SIZE& targetSize);
    static std::unique_ptr<UDPSenderWorker> createUDPSenderWorker(ThreadSafeQueue<std::vector<byte>>& frames,
        TCPConnection& connection, const u_short udpPort, const SIZE& targetSize, const ImageConfig& ic);
    static std::unique_ptr<ScreenCaptureWorker> createScreenCaptureWorker(ThreadSafeQueue<std::vector<byte>>& frames,
        const SIZE& targetSize, const int fps, const int maxDelayMs);
    static std::unique_ptr<RemoteEventExecutor> createRemoteEventExecutor(std::unique_ptr<TCPConnection> &&connection,
        const SIZE& targetSize);
};
