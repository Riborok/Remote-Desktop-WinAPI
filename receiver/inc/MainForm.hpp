#pragma once

#include "remote-desk/receiver/Receiver.hpp"
#include "Fonts.hpp"
#include "SimpleConfigDialogForm.hpp"

class MainForm {
    static constexpr auto WINDOWS_CLASS_NAME = L"ScreenshotReceiverClass";
    static constexpr int ID_MENU = 1;
    static constexpr int ID_TOGGLE_FULLSCREEN = 2;
    static ACCEL _accelTableEntries[];
    
    static HINSTANCE _hInstance;
    
    const Fonts& _fonts;
    HWND _hwnd;
    ReceiverConfig _config{};
    Receiver _receiver{};
    SimpleConfigDialogForm _configDialog;
    HACCEL _hAccel;
    WINDOWPLACEMENT _wpc;
    LONG _hwndStyle;
    LONG _hwndStyleEx;
    bool _isFullscreen;
public:
    static void registerClass(const HINSTANCE hInstance);

    explicit MainForm(const Fonts &fonts);
    void set(const ReceiverConfig &config, Receiver&& receiver);
    void show() const;
    HWND getHwnd() const;

    ~MainForm();
    MainForm(MainForm&&) = delete;
    MainForm& operator=(MainForm&&) = delete;
    MainForm(const MainForm&) = delete;
    MainForm& operator=(const MainForm&) = delete;
private:
    HWND createHwnd();
    void onConnectionClosed() const;
    static LRESULT CALLBACK windowProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
    LRESULT handleInput(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) const;
    void toggleFullscreen();
};
