#include "../inc/utils.hpp"

#include <stdexcept>

#include "../inc/ConfigDialogForm.hpp"

std::optional<SenderConfig> getConfig(const HINSTANCE hInstance, const Fonts& fonts) {
    SenderConfig config;
    ConfigDialogForm dialogForm(hInstance, fonts);
    return dialogForm.show(config) ? std::make_optional(config) : std::nullopt;
}

bool safeStoi(const wchar_t* str, int& result) {
    try {
        result = std::stoi(str);
        return true;
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}