#pragma once

#include "KeyboardControls.hpp"

namespace LibIO::Keyboard {
    class Windows final : public KeyboardControls {
    public:
        Windows() = default;

        ~Windows() override;

        void PressKey(std::string key) override;

        void Hotkey(std::string modifier, std::string key) override;
    };
}