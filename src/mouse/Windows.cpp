#include "LibIO/mouse/Windows.hpp"

#if PLATFORM_WINDOWS
#include <windows.h>

namespace LibIO::Mouse {

    Windows& Windows::getInstance() {
        static Linux instance;
        return instance;
    }

    void Windows::MoveCursor(const int x, const int y) {
        SetCursorPos(x, y);
    }

    void Windows::LeftClick() {
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }

    void Windows::RightClick() {
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
    }

    void Windows::MoveAndLeftClick(const int x, const int y) {
        MoveCursor(x, y);
        LeftClick();
    }

    void Windows::MoveAndRightClick(int x, int y) {
        MoveCursor(x, y);
        RightClick();
    }

    void Windows::Scroll(int amount, int* x = nullptr, int* y = nullptr) {
        if (x != nullptr && y != nullptr) {
            MoveCursor(*x, *y);
        }

        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, (UINT)(amount * WHEEL_DELTA), 0);
    }

    void Windows::ScrollUp(int amount) {
        Scroll(amount, nullptr, nullptr);
    }

    void Windows::ScrollDown(int amount) {
        Scroll(-amount,nullptr, nullptr);
    }
}
#endif