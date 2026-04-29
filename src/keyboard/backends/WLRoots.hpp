#pragma once

#include <unordered_map>

#include "LibIO/KeyboardControls.hpp"

namespace LibIO::Keyboard::Backends {
    class WLRoots: public KeyboardControls {
    public:
        ~WLRoots() override = default;
        WLRoots() = default;

        static KeyboardControls &getInstance();

        void PressKey(const std::string &key) override;
        void Hotkey(const std::string &modifier, const std::string &key) override;
    };
}
