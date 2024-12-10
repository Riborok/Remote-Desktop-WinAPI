#include "inc/MainForm.hpp"
#include "utils/sock/WinSockUtils.hpp"
#include "inc/ConfigDialogForm.hpp"
#include "inc/utils.hpp"

int WINAPI WinMain(const HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WinSockUtils::initializeWinSock();

    const Fonts fonts;
    MainForm::registerClass(hInstance);
    MainForm mainForm(fonts);
    if (auto params = getParams(hInstance, mainForm.getHwnd(), fonts)) {
        auto [config, receiver] = std::move(*params);
        mainForm.set(config, std::move(receiver));
        mainForm.show();
    }
    
    WinSockUtils::cleanupWinSock();
    return 0;
}
