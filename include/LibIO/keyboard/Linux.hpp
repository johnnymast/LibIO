#pragma once

#include <unordered_map>

#include "KeyboardControls.hpp"

#if defined(PLATFORM_LINUX)
#include <X11/Xlib.h>

namespace LibIO::Keyboard {
    class Linux final : public KeyboardControls {
    public:
        ~Linux() override = default;

        void PressKey(const std::string &key) override;
        void Hotkey(const std::string &modifier, const std::string &key) override;

    private:
        static std::unordered_map<char, std::pair<std::string, std::string>> SpecialCharacterMap;
        static std::unordered_map<std::string, uint> KeyCodes;
        static std::string ToLower(const std::string& input);
        static Display* GetDisplay();
    };
}
#endif