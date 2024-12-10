#include "../inc/utils.hpp"

#include "../inc/ConfigDialogForm.hpp"

std::optional<SenderConfig> getConfig(const HINSTANCE hInstance, const Fonts& fonts) {
    SenderConfig config;
    ConfigDialogForm dialogForm(hInstance, fonts);
    return dialogForm.show(config) ? std::make_optional(config) : std::nullopt;
}
