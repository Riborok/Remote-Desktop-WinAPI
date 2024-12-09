#include "inc/MainForm.hpp"
#include "utils/sock/WinSockUtils.hpp"
#include "inc/ConfigDialogForm.hpp"

int WINAPI WinMain(const HINSTANCE hInstance, const HINSTANCE hPrevInstance, const LPSTR lpCmdLine, const int nCmdShow) {
    WinSockUtils::initializeWinSock();

    const Fonts fonts;
    SenderConfig config;
    ConfigDialogForm dialogForm(hInstance, fonts);
    if (dialogForm.show(config)) {
        MainForm::registerClass(hInstance);
        const MainForm mainForm(fonts, config);
        mainForm.show();
    }
    
    WinSockUtils::cleanupWinSock();
    return 0;
}
