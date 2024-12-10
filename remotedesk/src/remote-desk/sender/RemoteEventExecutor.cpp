#include "../../../inc/remote-desk/sender/RemoteEventExecutor.hpp"

#include <stdexcept>

#include "../../../inc/utils/remote-desk/MessageUtils.hpp"
#include "../../../inc/utils/array/ByteArrayUtils.hpp"

RemoteEventExecutor::RemoteEventExecutor(std::unique_ptr<TCPConnection> &&connection, const SIZE& screenResolution):
        _connection(std::move(connection)),
        _mousePointScaler({screenResolution,
            {TypeLimits::MAX_WORD_VALUE, TypeLimits::MAX_WORD_VALUE}}),
        _disconnectCallback([]{ }){
    _connection->setReceiveTimeout(RECEIVE_TIMEOUT);
}

void RemoteEventExecutor::setDisconnectCallback(std::function<void()>&& callback) {
    _disconnectCallback = std::move(callback);
}

void RemoteEventExecutor::eventLoop() {
    while (_running) {
        handleEvent();
    }
}

void RemoteEventExecutor::handleEvent() {
    int bufferSize = EVENT_SIZE;
    const std::vector<byte> eventBuffer = receiveEvent(bufferSize);
    if (bufferSize == EVENT_SIZE) {
        const auto [message, wParam, lParam] = extractEventData(eventBuffer);
        INPUT input = createInput(message, wParam, lParam);
        if (input.type != UNKNOWN_TYPE) {
            SendInput(1, &input, sizeof(INPUT));
        }
    }
}

std::vector<byte> RemoteEventExecutor::receiveEvent(int& bufferSize) {
    try {
        return _connection->receiveData(bufferSize);
    } catch (const std::runtime_error&) {
        if (_running) {
            _running = false;
            _disconnectCallback();
        }
        bufferSize = 0;
        return {};
    }
}

std::tuple<UINT, WPARAM, LPARAM> RemoteEventExecutor::extractEventData(const std::vector<byte>& eventBuffer) {
    UINT message = ByteArrayUtils::getValueByByteIndex<UINT>(eventBuffer, 0);
    WPARAM wParam = ByteArrayUtils::getValueByByteIndex<WPARAM>(eventBuffer, sizeof(UINT));
    LPARAM lParam = ByteArrayUtils::getValueByByteIndex<LPARAM>(eventBuffer, sizeof(UINT) + sizeof(WPARAM));
    return {message, wParam, lParam};
}

INPUT RemoteEventExecutor::createInput(const UINT message, const WPARAM wParam, const LPARAM lParam) const {
    INPUT input = {};
    input.type = UNKNOWN_TYPE;
    if (MessageUtils::isKeyboardMessage(message)) {
        input = createKeyboardInput(message, wParam);
    }
    else if (MessageUtils::isMouseMessage(message)) {
        input = createMouseInput(message, wParam, lParam);
    }
    return input;
}

INPUT RemoteEventExecutor::createKeyboardInput(const UINT message, const WPARAM wParam) {
    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = LOWORD(wParam);
    input.ki.dwFlags |= (message == WM_KEYUP || message == WM_SYSKEYUP) ? KEYEVENTF_KEYUP : 0;
    return input;
}

INPUT RemoteEventExecutor::createMouseInput(const UINT message, const WPARAM wParam, const LPARAM lParam) const {
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags |= MOUSEEVENTF_ABSOLUTE;
    input.mi.dx = _mousePointScaler.scaleByX(LOWORD(lParam));
    input.mi.dy = _mousePointScaler.scaleByY(HIWORD(lParam));
        
    switch (message) {
    case WM_LBUTTONDOWN:
        input.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
        break;
    case WM_LBUTTONUP:
        input.mi.dwFlags |= MOUSEEVENTF_LEFTUP;
        break;
    case WM_RBUTTONDOWN:
        input.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN;
        break;
    case WM_RBUTTONUP:
        input.mi.dwFlags |= MOUSEEVENTF_RIGHTUP;
        break;
    case WM_MBUTTONDOWN:
        input.mi.dwFlags |= MOUSEEVENTF_MIDDLEDOWN;
        break;
    case WM_MBUTTONUP:
        input.mi.dwFlags |= MOUSEEVENTF_MIDDLEUP;
        break;
    case WM_MOUSEWHEEL: {
        input.mi.dwFlags |= MOUSEEVENTF_WHEEL;
        input.mi.mouseData = static_cast<DWORD>(GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    }
    case WM_MOUSEHWHEEL: {
        input.mi.dwFlags |= MOUSEEVENTF_HWHEEL;
        input.mi.mouseData = static_cast<DWORD>(GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    }
    case WM_MOUSEMOVE:
        input.mi.dwFlags |= MOUSEEVENTF_MOVE;
        break;
    default:
        input.type = UNKNOWN_TYPE;
        break;
    }

    return input;
}
