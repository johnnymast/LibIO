#pragma once

#if PLATFORM_WINDOWS
#   pragma message("✅ WINDOWS")
// #define MOUSE_CLASS() new ScreenshotWin32()
// #define KEYBOARD_CLASS() new ScreenshotWin32()
// #include "../../../src/windows/mouse.hpp"
// #include "../../../src/windows/keyboard.hpp
#include "LibIO/mouse/Windows.hpp"
#define MOUSE_CLASS() new LibIO::Mouse::Windows();

#elif PLATFORM_LINUX

    #include "LibIO/mouse/Linux.hpp"
    #include "LibIO/keyboard/Linux.hpp"
    #include "LibIO/clipboard/Linux.hpp"

    #define MouseControleInstance() new LibIO::Mouse::Linux();
    #define KeyboadControleInstance() new LibIO::Keyboard::Linux();
    #define ClipboardControleInstance() new LibIO::Clipboard::Linux();
//
// #include "../../../src/mouse/Linux.hpp"
// #include "../../../src/mouse/keyboard.hpp"
#else
// #   pragma message("✅ OOPS")
#define MouseControleInstance() nullptr
#define KeyboadControleInstance() nullptr
#endif
