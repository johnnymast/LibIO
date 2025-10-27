#pragma once

#include "KeyboardControls.hpp"

namespace LibIO::Keyboard {
    class Linux final : public KeyboardControls {
    public:
        Linux() = default;

        ~Linux() override;

        void PressKey(std::string key) override;

        void Hotkey(std::string modifier, std::string key) override;
    };
}
