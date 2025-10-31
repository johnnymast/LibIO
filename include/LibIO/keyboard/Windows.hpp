#pragma once

#include "KeyboardControls.hpp"

namespace LibIO::Keyboard {
    class Windows final : public KeyboardControls {
    public:

        ~Windows() override;
        static Windows &getInstance();
        static void Initialize();

        void PressKey(const std::string &key) override;
        void Hotkey(const std::string &modifier, const std::string &key) override;
    private:
        Windows() = default;
    };
}