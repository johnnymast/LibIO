#pragma once

#include "export.hpp"
#include <string>

namespace LibIO::Keyboard {
    class LIBIO_API  KeyboardControls {
    public:
        virtual ~KeyboardControls() = default;

        virtual void PressKey(const std::string& key) = 0;
        virtual void Hotkey(const std::string &modifier, const std::string &key) = 0;
    };
}