#include "LibIO/LibIO.hpp"


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