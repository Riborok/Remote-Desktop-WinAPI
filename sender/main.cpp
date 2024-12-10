#include "inc/MainForm.hpp"
#include "utils/sock/WinSockUtils.hpp"
#include "inc/ConfigDialogForm.hpp"
#include "inc/utils.hpp"

int WINAPI WinMain(const HINSTANCE hInstance, const HINSTANCE hPrevInstance, const LPSTR lpCmdLine, const int nCmdShow) {
    WinSockUtils::initializeWinSock();

    const Fonts fonts;
    if (const auto config = getConfig(hInstance, fonts)) {
        MainForm::registerClass(hInstance);
        const MainForm mainForm(fonts, *config);
        mainForm.show();
    }
    
    WinSockUtils::cleanupWinSock();
    return 0;
}
