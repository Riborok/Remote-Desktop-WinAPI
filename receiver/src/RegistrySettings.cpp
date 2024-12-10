#include "../inc/RegistrySettings.hpp"

const ReceiverConfig RegistrySettings::DEFAULT_RECEIVER_CONFIG = {
    0x0100007F,
    8080,
    8081,
    60,
    1000
};

void RegistrySettings::saveSettingsToRegistry(const ReceiverConfig& config) {
    HKEY hKey;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRY_PATH, 0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
        setRegistryValue(hKey, KEY_SERVER_IP, config.serverIp);
        setRegistryValue(hKey, KEY_SERVER_PORT, config.serverPort);
        setRegistryValue(hKey, KEY_UDP_PORT, config.udpPort);
        setRegistryValue(hKey, KEY_FPS, config.fps);
        setRegistryValue(hKey, KEY_MAX_DELAY_MS, config.maxDelayMs);

        RegCloseKey(hKey);
    }
}

void RegistrySettings::loadSettingsFromRegistry(ReceiverConfig& config) {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, REGISTRY_PATH, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD value;

        if (getRegistryValue(hKey, KEY_SERVER_IP, value))
            config.serverIp = value;

        if (getRegistryValue(hKey, KEY_SERVER_PORT, value))
            config.serverPort = static_cast<u_short>(value);

        if (getRegistryValue(hKey, KEY_UDP_PORT, value))
            config.udpPort = static_cast<u_short>(value);

        if (getRegistryValue(hKey, KEY_FPS, value))
            config.fps = static_cast<int>(value);

        if (getRegistryValue(hKey, KEY_MAX_DELAY_MS, value))
            config.maxDelayMs = static_cast<int>(value);

        RegCloseKey(hKey);
    } else {
        config = DEFAULT_RECEIVER_CONFIG;
    }
}

bool RegistrySettings::setRegistryValue(const HKEY hKey, const LPCWSTR key, const DWORD value) {
    return RegSetValueEx(hKey, key, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(value)) == ERROR_SUCCESS;
}

bool RegistrySettings::getRegistryValue(const HKEY hKey, const LPCWSTR key, DWORD& value) {
    DWORD dataSize = sizeof(value);
    return RegQueryValueEx(hKey, key, nullptr, nullptr, reinterpret_cast<BYTE*>(&value), &dataSize) == ERROR_SUCCESS;
}
