#include "WLRoots.hpp"

#if PLATFORM_WLROOTS
namespace LibIO::Clipboard::Backends {
    ClipboardControls & WLRoots::getInstance() {
    }

    void WLRoots::Copy(std::string text) {
    }

    std::string WLRoots::Paste() {
    }

    void WLRoots::Clear() {
    }
}
#endif
