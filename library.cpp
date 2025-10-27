#include "LibIO/internals/platform_dispatch.hpp"
#include "LibIO/LibIO.hpp"

using LibIO::Mouse::MouseControls;
using LibIO::Keyboard::KeyboardControls;

MouseControls LIBGRAPHICS_API *GetMouseControls() {
    return MouseControleInstance();
}

// KeyboardControls LIBGRAPHICS_API *GetKeyboardControls() {
//     return KeyboadControleInstance();
// }
