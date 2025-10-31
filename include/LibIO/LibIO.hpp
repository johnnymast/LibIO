#pragma once

#if defined(PLATFORM_WINDOWS)
#   pragma message("✅ WINDOWS")

    #include "LibIO/mouse/Windows.hpp"
    #include "LibIO/keyboard/Windows.hpp"

#elif defined(PLATFORM_LINUX)
#   pragma message("✅ Linux")

    #include "LibIO/mouse/Linux.hpp"
    #include "LibIO/keyboard/Linux.hpp"

#else
#   pragma message("✅ OOPS")

#endif

#include "LibIO/mouse/MouseControls.hpp"
#include "LibIO/keyboard/KeyboardControls.hpp"

using LibIO::Mouse::MouseControls;
using LibIO::Keyboard::KeyboardControls;

namespace LibIO {


    inline MouseControls LIBGRAPHICS_API *GetMouseControls() {
#if defined(PLATFORM_WINDOWS)
        return CreateMouseControlsInstance();
#elif defined(PLATFORM_LINUX)
        return LibIO::Mouse::Linux::getInstance();
#endif
    }

    inline KeyboardControls LIBGRAPHICS_API *GetKeyboardControls() {
        return CreateKeyboardControlsInstance();
    }
}