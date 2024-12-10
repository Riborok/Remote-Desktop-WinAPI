#pragma once

#include <optional>

#include "remote-desk/receiver/Receiver.hpp"
#include "Fonts.hpp"

std::optional<std::tuple<ReceiverConfig, Receiver>> getParams(const HINSTANCE hInstance, const HWND hwnd, const Fonts& fonts);
