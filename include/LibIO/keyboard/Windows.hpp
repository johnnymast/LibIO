#pragma once

#include "KeyboardControls.hpp"

#if defined(PLATFORM_WINDOWS)
namespace LibIO::Keyboard {
    class LIBIO_API Windows final : public KeyboardControls {
    public:
        ~Windows() override = default;

        static KeyboardControls &getInstance();

        std::string ToLower(const std::string &input);

        void PressKey(const std::string &key) override;

        void Hotkey(const std::string &modifier, const std::string &key) override;

    private:
        Windows() = default;
    };
}
#endif
