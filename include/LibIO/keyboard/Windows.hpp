#pragma once

#include <unordered_map>

#include "KeyboardControls.hpp"

#if defined(PLATFORM_WINDOWS)
namespace LibIO::Keyboard {
    class Windows final : public KeyboardControls {
    public:
        Windows() = default;
        ~Windows() override;

        void PressKey(const std::string &key) override;

        void Hotkey(const std::string &modifier, const std::string &key) override;

    private:
        static const std::unordered_map<std::string, uint8_t> KeyCodes;
        std::string ToLower(const std::string &input);
    };
}
#endif
