#pragma once
#include "LibIO/export.hpp"

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
#include "LibIO/clipboard/ClipboardControls.hpp"

using LibIO::Mouse::MouseControls;
using LibIO::Keyboard::KeyboardControls;
using LibIO::Clipboard::ClipboardControls;

namespace LibIO {
    LIBIO_API MouseControls *GetMouseControls();
    LIBIO_API KeyboardControls  *GetKeyboardControls();
    LIBIO_API ClipboardControls  *GetClipboardControls();
}