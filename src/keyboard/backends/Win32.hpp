#pragma once
#include "LibIO/KeyboardControls.hpp"

#if PLATFORM_WINDOWS
#include "LibIO/export.hpp"

namespace LibIO::Keyboard::Backends {
    class LIBIO_API Win32 final: public KeyboardControls {
    public:
        ~Win32() override = default;

        static KeyboardControls &getInstance();

        std::string ToLower(const std::string &input);

        void PressKey(const std::string &key) override;
        void Hotkey(const std::string &modifier, const std::string &key) override;
    };
}
#endif