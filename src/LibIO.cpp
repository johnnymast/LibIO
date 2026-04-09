#include "LibIO/LibIO.hpp"

// using LibIO::Mouse::MouseControls;
// using LibIO::Keyboard::KeyboardControls;
using LibIO::Clipboard::ClipboardControls;

namespace LibIO {

    MouseControls *GetMouseControls() {
#if defined(PLATFORM_WINDOWS)
        return &LibIO::Mouse::Windows::getInstance();
#elif defined(PLATFORM_LINUX)
        return &LibIO::Mouse::Linux::getInstance();
#else
        return nullptr;
#endif
    }

    KeyboardControls *GetKeyboardControls() {
#if defined(PLATFORM_WINDOWS)
        return &LibIO::Keyboard::Windows::getInstance();
#elif defined(PLATFORM_LINUX)
        return &LibIO::Keyboard::Linux::getInstance();
#else
        return nullptr;
#endif
    }

    ClipboardControls *GetClipboardControls() {
#if defined(PLATFORM_WINDOWS)
        return &LibIO::Clipboard::Windows::getInstance();
#elif defined(PLATFORM_LINUX)
        return &LibIO::Clipboard::Linux::getInstance();
#else
        return nullptr;
#endif
    }
}