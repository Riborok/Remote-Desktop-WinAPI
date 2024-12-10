#pragma once

#include "remote-desk/receiver/Receiver.hpp"
#include "Fonts.hpp"
#include "SimpleConfigDialogForm.hpp"

class MainForm {
    static constexpr auto WINDOWS_CLASS_NAME = L"ScreenshotReceiverClass";

    static HINSTANCE _hInstance;
    const Fonts& _fonts;
    ReceiverConfig _config;
    Receiver _receiver;
    HWND _hwnd;
    SimpleConfigDialogForm _configDialog;
public:
    static void registerClass(const HINSTANCE hInstance);
    
    MainForm(const Fonts &fonts);
    void set(const ReceiverConfig &config, Receiver&& receiver);
    void show() const;
    HWND getHwnd() const;

    ~MainForm() = default;
    MainForm(MainForm&&) = delete;
    MainForm& operator=(MainForm&&) = delete;
    MainForm(const MainForm&) = delete;
    MainForm& operator=(const MainForm&) = delete;
private:
    static LRESULT CALLBACK windowProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
};
