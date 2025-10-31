#pragma once

#include <unordered_map>

#include "KeyboardControls.hpp"

#if defined(PLATFORM_WINDOWS)
namespace LibIO::Keyboard {
    class Windows final : public KeyboardControls {
    public:

        Windows() = default;
        ~Windows() override;

        static KeyboardControls &getInstance();

        void PressKey(const std::string &key) override;
        void Hotkey(const std::string &modifier, const std::string &key) override;
    };
}
#endif
