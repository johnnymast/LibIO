#pragma once
#include "clipboard/ClipboardControls.hpp"

#if defined(PLATFORM_WINDOWS)

    #include "LibIO/mouse/Windows.hpp"
    #include "LibIO/keyboard/Windows.hpp"
    #include "LibIO/clipboard/Windows.hpp"

#elif defined(PLATFORM_LINUX)

    #include "LibIO/mouse/Linux.hpp"
    #include "LibIO/keyboard/Linux.hpp"
    #include "LibIO/clipboard/Linux.hpp"

#endif

#include "LibIO/mouse/MouseControls.hpp"
#include "LibIO/keyboard/KeyboardControls.hpp"

using LibIO::Mouse::MouseControls;
using LibIO::Keyboard::KeyboardControls;
using LibIO::Clipboard::ClipboardControls;

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

    inline ClipboardControls LIBGRAPHICS_API *GetClipboardControls() {
#if defined(PLATFORM_WINDOWS)
        return LibIO::Clipboard::Windows::getInstance();
#elif defined(PLATFORM_LINUX)
        return &LibIO::Clipboard::Linux::getInstance();
#else
        return nullptr;
#endif
    }

}