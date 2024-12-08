#include "../inc/RegistrySettings.hpp"

#include <windows.h>

#include "utils/screen/ScreenUtils.hpp"

SenderCfg const RegistrySettings::DEFAULT_SENDER_CFG = {
    8080,
    {
        ImageFormat::jpg,
        60
    },
    30,
    1000,
    ScreenUtils::getScreenSize()
};

void RegistrySettings::saveSettingsToRegistry(const SenderCfg& config) {
    HKEY hKey;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRY_PATH, 0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
        setRegistryValue(hKey, KEY_PORT, config.tcpServerPort);
        setRegistryValue(hKey, KEY_FPS, static_cast<DWORD>(config.fps));
        setRegistryValue(hKey, KEY_MAX_DELAY_MS, static_cast<DWORD>(config.maxDelayMs));
        setRegistryValue(hKey, KEY_QUALITY, static_cast<DWORD>(config.imageConfig.quality));
        setRegistryValue(hKey, KEY_WIDTH, static_cast<DWORD>(config.targetSize.cx));
        setRegistryValue(hKey, KEY_HEIGHT, static_cast<DWORD>(config.targetSize.cy));
        setRegistryValue(hKey, KEY_FORMAT, static_cast<DWORD>(config.imageConfig.ext));

        RegCloseKey(hKey);
    }
}

void RegistrySettings::loadSettingsFromRegistry(SenderCfg& config) {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, REGISTRY_PATH, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD value;

        if (getRegistryValue(hKey, KEY_PORT, value))
            config.tcpServerPort = static_cast<u_short>(value);

        if (getRegistryValue(hKey, KEY_FPS, value))
            config.fps = static_cast<int>(value);

        if (getRegistryValue(hKey, KEY_MAX_DELAY_MS, value))
            config.maxDelayMs = static_cast<int>(value);

        if (getRegistryValue(hKey, KEY_FORMAT, value))
            config.imageConfig.ext = static_cast<ImageFormat>(value);

        if (getRegistryValue(hKey, KEY_QUALITY, value))
            config.imageConfig.quality = static_cast<int>(value);

        if (getRegistryValue(hKey, KEY_WIDTH, value))
            config.targetSize.cx = static_cast<int>(value);

        if (getRegistryValue(hKey, KEY_HEIGHT, value))
            config.targetSize.cy = static_cast<int>(value);

        RegCloseKey(hKey);
    } else {
        config = DEFAULT_SENDER_CFG;
    }
}

bool RegistrySettings::setRegistryValue(const HKEY hKey, const LPCWSTR key, const DWORD value) {
    return RegSetValueEx(hKey, key, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(value)) == ERROR_SUCCESS;
}

bool RegistrySettings::getRegistryValue(const HKEY hKey, const LPCWSTR key, DWORD& value) {
    DWORD dataSize = sizeof(value);
    return RegQueryValueEx(hKey, key, nullptr, nullptr, reinterpret_cast<BYTE*>(&value), &dataSize) == ERROR_SUCCESS;
}