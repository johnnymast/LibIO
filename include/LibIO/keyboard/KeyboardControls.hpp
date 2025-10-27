#pragma once

#include <string>

namespace LibIO::Keyboard {
    class KeyboardControls {
    public:
        virtual ~KeyboardControls() = default;

        virtual void PressKey(std::string key) = 0;

        virtual void Hotkey(std::string modifier, std::string key) = 0;
    };
}