#include "../../../inc/screen/render/ScreenRenderWorker.hpp"

ScreenRenderWorker::ScreenRenderWorker(ThreadSafeQueue<MaskedData>& queue,
        const HWND hWnd, SIZE& appSize, const SIZE& receivedSize, const int fps) :
    Worker(fps), _queue(queue), _screenRender(hWnd, appSize, receivedSize) { }

void ScreenRenderWorker::process() {
    _screenRender.render(*_queue.dequeue());
}
