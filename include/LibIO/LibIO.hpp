#pragma once

#if defined(PLATFORM_WINDOWS)
#   pragma message("✅ WINDOWS")

    #include "LibIO/mouse/Windows.hpp"
    #include "LibIO/keyboard/Windows.hpp"

    #define MouseControleInstance() new LibIO::Mouse::Windows()->getInstance();
    #define KeyboadControleInstance() new LibIO::Keyboard::Windows()->getInstance();

#elif defined(PLATFORM_LINUX)
// #   pragma message("✅ Linux")

    #include "LibIO/mouse/Linux.hpp"
    #include "LibIO/keyboard/Linux.hpp"

    #define MouseControleInstance() new LibIO::Mouse::Linux()->getInstance();
    #define KeyboadControleInstance() new LibIO::Keyboard::Linux()->getInstance();

#else
    #   pragma message("✅ OOPS")

    #define MouseControleInstance() nullptr
    #define KeyboadControleInstance() nullptr
#endif

#include "LibIO/mouse/MouseControls.hpp"
#include "LibIO/keyboard/KeyboardControls.hpp"

using LibIO::Mouse::MouseControls;
using LibIO::Keyboard::KeyboardControls;

namespace LibIO {
    inline MouseControls LIBGRAPHICS_API *GetMouseControls() {
        return MouseControleInstance();
    }

    inline KeyboardControls LIBGRAPHICS_API *GetKeyboardControls() {
        return KeyboadControleInstance();
    }
}
