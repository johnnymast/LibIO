#include "WLRoots.hpp"

#if PLATFORM_WLROOTS
namespace LibIO::Keyboard::Backends {
    LibIO::Keyboard::KeyboardControls & LibIO::Keyboard::Backends::WLRoots::getInstance() {
    }

    void LibIO::Keyboard::Backends::WLRoots::PressKey(const std::string &key) {
    }

    void LibIO::Keyboard::Backends::WLRoots::Hotkey(const std::string &modifier, const std::string &key) {
    }
}
#endif
