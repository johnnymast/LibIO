#include "Win32.hpp"

#if PLATFORM_WINDOWS
#include <windows.h>

namespace LibIO::Mouse::Backends {
    MouseControls& Win32::getInstance() {
        static Win32 instance;
        return instance;
    }

    void Win32::ClickButton(int button) {
        LeftClick();
    }

    void Win32::MoveCursor(const int x, const int y) {
        SetCursorPos(x, y);
    }

    void Win32::LeftClick() {
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }

    void Win32::RightClick() {
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
    }

    void Win32::MoveAndLeftClick(const int x, const int y) {
        MoveCursor(x, y);
        LeftClick();
    }

    void Win32::MoveAndRightClick(int x, int y) {
        MoveCursor(x, y);
        RightClick();
    }

    void Win32::Scroll(int amount, int* x, int* y) {
        if (x != nullptr && y != nullptr) {
            MoveCursor(*x, *y);
        }

        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, (UINT)(amount * WHEEL_DELTA), 0);
    }

    void Win32::ScrollUp(int amount) {
        Scroll(amount, nullptr, nullptr);
    }

    void Win32::ScrollDown(int amount) {
        Scroll(-amount,nullptr, nullptr);
    }
}
#endif