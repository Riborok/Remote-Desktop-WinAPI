#pragma once
#include "remote-desk/receiver/ReceiverConfig.hpp"

class RegistrySettings {
    static constexpr auto REGISTRY_PATH = L"Software\\RemoteDesktop\\Receiver";

    static constexpr auto KEY_SERVER_IP = L"ServerIp";
    static constexpr auto KEY_SERVER_PORT = L"ServerPort";
    static constexpr auto KEY_UDP_PORT = L"UdpPort";
    static constexpr auto KEY_FPS = L"FPS";
    static constexpr auto KEY_MAX_DELAY_MS = L"MaxDelayMs";

    static const ReceiverConfig DEFAULT_RECEIVER_CONFIG;
public:
    RegistrySettings() = delete;

    static void saveSettingsToRegistry(const ReceiverConfig& config);
    static void loadSettingsFromRegistry(ReceiverConfig& config);
private:
    static bool setRegistryValue(const HKEY hKey, const LPCWSTR key, const DWORD value);
    static bool getRegistryValue(const HKEY hKey, const LPCWSTR key, DWORD& value);
};
