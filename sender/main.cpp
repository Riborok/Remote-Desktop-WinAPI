#include "inc/MainForm.hpp"
#include "utils/sock/WinSockUtils.hpp"
#include "inc/ConfigDialogForm.hpp"

int WINAPI WinMain(const HINSTANCE hInstance, const HINSTANCE hPrevInstance, const LPSTR lpCmdLine, const int nCmdShow) {
    WinSockUtils::initializeWinSock();

    const Fonts fonts;
    SenderCfg cfg;
    ConfigDialogForm dialogForm(hInstance, fonts);
    if (dialogForm.show(cfg)) {
        MainForm::registerClass(hInstance);
        const MainForm mainForm(fonts, cfg);
        mainForm.show();
    }
    
    WinSockUtils::cleanupWinSock();
    return 0;
}
