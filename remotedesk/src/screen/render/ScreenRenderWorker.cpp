#include "../../../inc/screen/render/ScreenRenderWorker.hpp"

ScreenRenderWorker::ScreenRenderWorker(ThreadSafeQueue<std::vector<byte>>& queue,
        const HWND hWnd, const SIZE& appSize, const SIZE& receivedSize, const int fps, const int maxDelayMs) :
    FrameWorker(queue, fps, maxDelayMs), _screenRender(hWnd, appSize, receivedSize) { }

void ScreenRenderWorker::updateAppSize(const SIZE& appSize) {
    _screenRender.updateAppSize(appSize);
}

void ScreenRenderWorker::process() {
    _screenRender.render(*getFrames().dequeue());
}
