﻿#pragma once

#include "remote-desk/sender/Sender.hpp"
#include "./Fonts.hpp"

class MainForm {
    static constexpr auto WINDOWS_CLASS_NAME = L"ScreenshotSenderClass";
    static constexpr int BTN_APPLY_ID = 1;
    static constexpr int BTN_EXIT_ID = 2;

    static HINSTANCE _hInstance;

    HWND _hEditFps = nullptr; 
    HWND _hEditMaxDelay = nullptr; 
    HWND _hEditQuality = nullptr; 
    HWND _hComboBoxFormat = nullptr;
    const Fonts &_fonts;
    SenderConfig _config;
    HWND _hwnd;
    Sender _sender;
public:
    static void registerClass(const HINSTANCE hInstance);

    MainForm(const Fonts &fonts, const SenderConfig &config);
    void show() const;

    ~MainForm() = default;
    MainForm(MainForm&&) = delete;
    MainForm& operator=(MainForm&&) = delete;
    MainForm(const MainForm&) = delete;
    MainForm& operator=(const MainForm&) = delete;
private:
    HWND createHwnd();
    void onConnectionClosed() const;
    static LRESULT CALLBACK windowProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
    void createControls(const HWND hwnd);
    void updateConfig();
};
