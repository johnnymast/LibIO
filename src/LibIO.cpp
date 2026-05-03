#include "LibIO/LibIO.hpp"


#if defined(PLATFORM_WINDOWS)

#include "keyboard/backends/Win32.hpp"
#include "mouse/backends/Win32.hpp"
#elif defined(PLATFORM_X11)
#include "keyboard/backends/X11.hpp"
#include "mouse/backends/X11.hpp"
#endif


#if defined(PLATFORM_WLROOTS)
#endif

#if defined(WINDOWS_CLIPBOARD)
#include "clipboard/backends/Win32.hpp"
#endif

#if defined(X11_CLIPBOARD)
#include "clipboard/backends/X11.hpp"
#endif

#if defined(WLROOTS_CLIPBOARD)
#include "clipboard/backends/WLRoots.hpp"
#endif

#if defined(WLROOTS_KEYBOARD)
#include "keyboard/backends/WLRoots.hpp"
#endif

#if defined(WLROOTS_MOUSE)

#include "mouse/backends/WLRoots.hpp"
#endif


namespace LibIO {
    MouseControls *GetMouseControls() {
#if defined(PLATFORM_WINDOWS)
        return &Mouse::Backends::Win32::getInstance();
#elif defined(PLATFORM_X11)
        return &Mouse::Backends::X11::getInstance();
#elif defined(WLROOTS_MOUSE)
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
#elif defined(WLROOTS_KEYBOARD)
        return &Keyboard::Backends::WLRoots::getInstance();
#else
        return nullptr;
#endif
    }

    ClipboardControls *GetClipboardControls() {
#if defined(WINDOWS_CLIPBOARD)
        return &Clipboard::Backends::Win32::getInstance();
#elif defined(X11_CLIPBOARD)
        return &Clipboard::Backends::X11::getInstance();
#elif defined(WLROOTS_CLIPBOARD)
        return &Clipboard::Backends::WLRoots::getInstance();
#else
        return nullptr;
#endif
    }
}
