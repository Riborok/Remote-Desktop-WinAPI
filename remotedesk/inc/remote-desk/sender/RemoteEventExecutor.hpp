#pragma once

#include "../../sock/tcp/TCPConnection.hpp"
#include "../../utils/array/ByteArrayUtils.hpp"
#include "../../utils/TypeLimits.hpp"
#include "../../utils/screen/PointScaler.hpp"
#include "../../thread/ThreadWorker.hpp"

class RemoteEventExecutor final : public ThreadWorker {
    static constexpr DWORD RECEIVE_TIMEOUT = 1000;
    static constexpr size_t EVENT_SIZE = sizeof(UINT) + sizeof(WPARAM) + sizeof(LPARAM);
    static constexpr DWORD UNKNOWN_TYPE = TypeLimits::MAX_DWORD_VALUE;
    
    std::unique_ptr<TCPConnection> _connection;
    PointScaler _mousePointScaler;
public:
    explicit RemoteEventExecutor(std::unique_ptr<TCPConnection> &&connection, const SIZE& screenResolution);
protected:
    void eventLoop() override;
private:
    void handleEvent();
    
    std::vector<byte> receiveEvent();
    static std::tuple<UINT, WPARAM, LPARAM> extractEventData(const std::vector<byte>& eventBuffer);
    
    INPUT createInput(const UINT message, const WPARAM wParam, const LPARAM lParam) const;
    static bool isKeyboardMessage(const UINT message);
    static bool isMouseMessage(const UINT message);
    static INPUT createKeyboardInput(const UINT message, const WPARAM wParam);
    INPUT createMouseInput(const UINT message, const WPARAM wParam, const LPARAM lParam) const;
};
