#pragma once
#include "LibIO/export.hpp"

#if defined(PLATFORM_WINDOWS)
    #include "clipboard/backends/Win32.hpp"
    #include "keyboard/backends/Win32.hpp"
    #include "mouse/backends/Win32.hpp"
#elif defined(PLATFORM_X11)
    #include "clipboard/backends/X11.hpp"
    #include "keyboard/backends/X11.hpp"
    #include "mouse/backends/X11.hpp"
#endif

#if defined(PLATFORM_WLROOTS)
    #include "mouse/backends/WLRoots.hpp"
    #include "keyboard/backends/WLRoots.hpp"
    #include "clipboard/backends/WLRoots.hpp"
#endif

#include "LibIO/MouseControls.hpp"
#include "LibIO/KeyboardControls.hpp"
#include "LibIO/ClipboardControls.hpp"

using LibIO::Mouse::MouseControls;
using LibIO::Keyboard::KeyboardControls;
using LibIO::Clipboard::ClipboardControls;

namespace LibIO {
    LIBIO_API MouseControls *GetMouseControls();
    LIBIO_API KeyboardControls  *GetKeyboardControls();
    LIBIO_API ClipboardControls  *GetClipboardControls();
}