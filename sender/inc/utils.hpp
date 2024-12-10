#pragma once

#include <optional>

#include "Fonts.hpp"
#include "remote-desk/sender/SenderConfig.hpp"

std::optional<SenderConfig> getConfig(const HINSTANCE hInstance, const Fonts& fonts);
