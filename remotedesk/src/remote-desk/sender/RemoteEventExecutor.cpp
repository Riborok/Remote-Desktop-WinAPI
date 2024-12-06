#include "../../../inc/remote-desk/sender/RemoteEventExecutor.hpp"

#include <stdexcept>

RemoteEventExecutor::RemoteEventExecutor(std::unique_ptr<TCPConnection> &&connection, const SIZE& screenResolution)
    : _connection(std::move(connection)),
    _mousePointScaler({screenResolution,
        {TypeLimits::MAX_WORD_VALUE, TypeLimits::MAX_WORD_VALUE}}) {
    _connection->setReceiveTimeout(RECEIVE_TIMEOUT);
}

void RemoteEventExecutor::eventLoop() {
    while (_running) {
        handleEvent();
    }
}

void RemoteEventExecutor::handleEvent() {
    const std::vector<byte> eventBuffer = receiveEvent();
    if (eventBuffer.size() != EVENT_SIZE) {
        return;
    }
    const auto [message, wParam, lParam] = extractEventData(eventBuffer);
    INPUT input = createInput(message, wParam, lParam);
    if (input.type != UNKNOWN_TYPE) {
        SendInput(1, &input, sizeof(INPUT));
    }
}

std::vector<byte> RemoteEventExecutor::receiveEvent() {
    try {
        int bufferSize = EVENT_SIZE;
        std::vector<byte> buffer = _connection->receiveData(bufferSize);
        if (bufferSize == EVENT_SIZE) {
            return buffer;
        }
    } catch (const std::runtime_error&) {
        _running = false;
    }
    return {};
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
    if (isKeyboardMessage(message)) {
        input = createKeyboardInput(message, wParam);
    }
    else if (isMouseMessage(message)) {
        input = createMouseInput(message, wParam, lParam);
    }
    return input;
}

bool RemoteEventExecutor::isKeyboardMessage(const UINT message) {
    return message >= WM_KEYFIRST && message <= WM_KEYLAST;
}

bool RemoteEventExecutor::isMouseMessage(const UINT message) {
    return message >= WM_MOUSEFIRST && message <= WM_MOUSELAST;
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
