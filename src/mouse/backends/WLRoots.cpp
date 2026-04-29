#include "WLRoots.hpp"

#if PLATFORM_WLROOTS
namespace LibIO::Mouse::Backends {
    MouseControls & WLRoots::getInstance() {
    }

    void WLRoots::ClickButton(int button) {
    }

    void WLRoots::MoveCursor(int x, int y) {
    }

    void WLRoots::LeftClick() {
    }

    void WLRoots::RightClick() {
    }

    void WLRoots::MoveAndLeftClick(int x, int y) {
    }

    void WLRoots::MoveAndRightClick(int x, int y) {
    }

    void WLRoots::Scroll(int amount, int *x, int *y) {
    }

    void WLRoots::ScrollUp(int amount) {
    }
}
#endif
