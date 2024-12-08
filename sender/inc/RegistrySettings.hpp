#pragma once

#include "../../remotedesk/src/remote-desk/sender/SenderCfg.hpp"

class RegistrySettings {
    static constexpr auto REGISTRY_PATH = L"Software\\RemoteDesktop\\Sender";

    static constexpr auto KEY_PORT = L"Port";
    static constexpr auto KEY_FPS = L"FPS";
    static constexpr auto KEY_MAX_DELAY_MS = L"MaxDelayMs";
    static constexpr auto KEY_FORMAT = L"Format";
    static constexpr auto KEY_QUALITY = L"Quality";
    static constexpr auto KEY_WIDTH = L"Width";
    static constexpr auto KEY_HEIGHT = L"Height";

    static const SenderCfg DEFAULT_SENDER_CFG;
public:
    RegistrySettings() = delete;
    
    static void saveSettingsToRegistry(const SenderCfg& config);
    static void loadSettingsFromRegistry(SenderCfg& config);
private:
    static bool setRegistryValue(const HKEY hKey, const LPCWSTR key, const DWORD value);
    static bool getRegistryValue(const HKEY hKey, const LPCWSTR key, DWORD& value);
};
