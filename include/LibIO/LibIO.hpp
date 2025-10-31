#pragma once

#if defined(PLATFORM_WINDOWS)

    #include "LibIO/mouse/Windows.hpp"
    #include "LibIO/keyboard/Windows.hpp"

#elif defined(PLATFORM_LINUX)

    #include "LibIO/mouse/Linux.hpp"
    #include "LibIO/keyboard/Linux.hpp"

#endif

#include "LibIO/mouse/MouseControls.hpp"
#include "LibIO/keyboard/KeyboardControls.hpp"

using LibIO::Mouse::MouseControls;
using LibIO::Keyboard::KeyboardControls;

namespace LibIO {


    inline MouseControls LIBGRAPHICS_API *GetMouseControls() {
#if defined(PLATFORM_WINDOWS)
        return return LibIO::Mouse::Windows::getInstance();
#elif defined(PLATFORM_LINUX)
        return &LibIO::Mouse::Linux::getInstance();
#else
        return nullptr;
#endif
    }

    inline KeyboardControls LIBGRAPHICS_API *GetKeyboardControls() {
#if defined(PLATFORM_WINDOWS)
        return LibIO::Keyboard::Windows::getInstance();
#elif defined(PLATFORM_LINUX)
        return &LibIO::Keyboard::Linux::getInstance();
#else
        return nullptr;
#endif
    }
}