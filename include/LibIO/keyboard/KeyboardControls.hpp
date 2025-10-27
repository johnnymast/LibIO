#pragma once

#include <string>

namespace LibIO::Keyboard {
    class KeyboardControls {
    public:
        virtual ~KeyboardControls() = default;

        virtual void PressKey(const std::string& key);

        virtual void Hotkey(const std::string &modifier, const std::string &key);
    };
}