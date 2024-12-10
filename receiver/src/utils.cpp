#include "../inc/utils.hpp"

#include "../inc/ConfigDialogForm.hpp"

std::optional<Receiver> tryCreateReceiver(const HWND hwnd, const ReceiverConfig& config) {
    try {
        return std::make_optional(Receiver(hwnd, config));
    } catch (const std::runtime_error&) {
        return std::nullopt;
    }
}

std::optional<std::tuple<ReceiverConfig, Receiver>> getParams(const HINSTANCE hInstance, const HWND hwnd, const Fonts& fonts) {
    ConfigDialogForm dialogForm(hInstance, fonts);
    
    ReceiverConfig config;
    while (true) {
        if (!dialogForm.show(config)) {
            return std::nullopt;
        }
        if (auto receiver = tryCreateReceiver(hwnd, config)) {
            return std::make_tuple(config, std::move(*receiver));
        }
        MessageBox(hwnd, L"Failed to connect to the server", L"Connection Error", MB_ICONERROR | MB_OK);
    }
}