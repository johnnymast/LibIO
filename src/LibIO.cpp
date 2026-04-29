#include "LibIO/LibIO.hpp"

namespace LibIO {

    MouseControls *GetMouseControls() {
#if defined(PLATFORM_WINDOWS)
        return &Mouse::Backends::Win32::getInstance();
#elif defined(PLATFORM_X11)
        return &Mouse::Backends::X11::getInstance();
#elif defined(PLATFORM_WLROOTS)
        return &Mouse::Backends::WLRoots::getInstance();
#else
        return nullptr;
#endif
    }

    KeyboardControls *GetKeyboardControls() {
#if defined(PLATFORM_WINDOWS)
        return &Keyboard::Backends::Win32::getInstance();
#elif defined(PLATFORM_X11)
        return &Keyboard::Backends::X11::getInstance();
#elif defined(PLATFORM_WLROOTS)
        return &Keyboard::Backends::WLRoots::getInstance();
#else
        return nullptr;
#endif
    }

    ClipboardControls *GetClipboardControls() {
#if defined(PLATFORM_WINDOWS)
        return &Clipboard::Backends::Win32::getInstance();
#elif defined(PLATFORM_X11)
        return &Clipboard::Backends::X11::getInstance();
#elif defined(PLATFORM_WLROOTS)
        return &Clipboard::Backends::WLRoots::getInstance();
#else
        return nullptr;
#endif
    }
}