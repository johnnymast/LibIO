#pragma once

#include "LibIO/KeyboardControls.hpp"

#if PLATFORM_X11
#include <unordered_map>
#include <X11/Xlib.h>

namespace LibIO::Keyboard::Backends {
    class X11 : public KeyboardControls {
    public:
        X11() = default;

        ~X11() override = default;

        static KeyboardControls &getInstance();

        void PressKey(const std::string &key) override;

        void Hotkey(const std::string &modifier, const std::string &key) override;

    private:
        static std::unordered_map<char, std::pair<std::string, std::string> > SpecialCharacterMap;
        static std::unordered_map<std::string, uint> KeyCodes;

        static std::string ToLower(const std::string &input);

        static Display *GetDisplay();
    };
}
#endif
