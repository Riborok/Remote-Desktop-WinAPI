#pragma once

#include "./remote-desk/sender/SenderConfig.hpp"

class RegistrySettings {
    static constexpr auto REGISTRY_PATH = L"Software\\RemoteDesktop\\Sender";

    static constexpr auto KEY_PORT = L"Port";
    static constexpr auto KEY_FPS = L"FPS";
    static constexpr auto KEY_MAX_DELAY_MS = L"MaxDelayMs";
    static constexpr auto KEY_FORMAT = L"Format";
    static constexpr auto KEY_QUALITY = L"Quality";
    static constexpr auto KEY_WIDTH = L"Width";
    static constexpr auto KEY_HEIGHT = L"Height";

    static const SenderConfig DEFAULT_SENDER_CONFIG;
public:
    RegistrySettings() = delete;
    
    static void saveSettingsToRegistry(const SenderConfig& config);
    static void loadSettingsFromRegistry(SenderConfig& config);
private:
    static bool setRegistryValue(const HKEY hKey, const LPCWSTR key, const DWORD value);
    static bool getRegistryValue(const HKEY hKey, const LPCWSTR key, DWORD& value);
};
