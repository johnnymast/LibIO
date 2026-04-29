#include "X11.hpp"


#if PLATFORM_X11
#include <cstdint>
#include <stdexcept>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <iostream>

namespace LibIO::Mouse::Backends {
    MouseControls &X11::getInstance() {
        static X11 instance;
        return instance;
    }

    Display *X11::GetDisplay() {
        Display *display = XOpenDisplay(nullptr);
        if (display == nullptr) {
            throw std::runtime_error("Could not open X11 display");
        }
        return display;
    }

    void X11::ClickButton(const int button) {
        Display *display = GetDisplay();
        try {
            XTestFakeButtonEvent(display, button, True, 0); // Press
            XTestFakeButtonEvent(display, button, False, 0); // Release
            XFlush(display);
        } catch (...) {
            XCloseDisplay(display);
            throw;
        }
        XCloseDisplay(display);
    }

    void X11::MoveCursor(const int x, const int y) {
        Display *display = GetDisplay();
        try {
            XTestFakeMotionEvent(display, 0, x, y, 0);
            XFlush(display);
        } catch (...) {
            XCloseDisplay(display);
            throw;
        }
        XCloseDisplay(display);
    }

    void X11::LeftClick() {
        ClickButton(Button1);
    }

    void X11::RightClick() {
        ClickButton(Button3);
    }

    void X11::MoveAndLeftClick(const int x, const int y) {
        Display *display = GetDisplay();
        try {
            MoveCursor(x, y);
            XTestFakeButtonEvent(display, Button1, True, 0);
            XTestFakeButtonEvent(display, Button1, False, 0);
            XFlush(display);
        } catch (...) {
            XCloseDisplay(display);
            throw;
        }
        XCloseDisplay(display);
    }

    void X11::MoveAndRightClick(const int x, const int y) {
        Display *display = GetDisplay();
        try {
            MoveCursor(x, y);
            XTestFakeButtonEvent(display, Button3, True, 0);
            XTestFakeButtonEvent(display, Button3, False, 0);
            XFlush(display);
        } catch (...) {
            XCloseDisplay(display);
            throw;
        }
        XCloseDisplay(display);
    }

    void X11::Scroll(const int amount, int *x, int *y) {
        Display *display = GetDisplay();
        try {
            if (x != nullptr && y != nullptr) {
                XTestFakeMotionEvent(display, 0, *x, *y, 0);
            }

            const uint32_t button = amount > 0 ? Button4 : Button5;
            const int clicks = std::abs(amount);

            for (int i = 0; i < clicks; i++) {
                XTestFakeButtonEvent(display, button, True, 0);
                XTestFakeButtonEvent(display, button, False, 0);
            }

            XFlush(display);
        } catch (...) {
            XCloseDisplay(display);
            throw;
        }
        XCloseDisplay(display);
    }

    void X11::ScrollUp(const int amount) {
        Scroll(amount, nullptr, nullptr);
    }

    void X11::ScrollDown(const int amount) {
        Scroll(-amount, nullptr, nullptr);
    }
}
#endif
