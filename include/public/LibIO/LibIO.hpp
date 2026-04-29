#pragma once


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