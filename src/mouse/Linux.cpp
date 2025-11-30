#include "LibIO/mouse/Linux.hpp"
#include "LibIO/mouse/MouseControls.hpp"

#if PLATFORM_LINUX
#include <cstdint>
#include <stdexcept>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

namespace LibIO::Mouse {

    MouseControls& Linux::getInstance() {
        static Linux instance;
        return instance;
    }

    Display* Linux::GetDisplay() {
        Display* display = XOpenDisplay(nullptr);
        if (display == nullptr) {
            throw std::runtime_error("Could not open X11 display");
        }
        return display;
    }

    void Linux::ClickButton(const int button) {
        Display* display = GetDisplay();
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

    void Linux::MoveCursor(const int x, const int y) {
        Display *display = GetDisplay();
        try {
            // #if WAYLAND
            // Huidige cursorpositie ophalen
            Window root = DefaultRootWindow(display);
            Window ret_root, ret_child;
            int root_x, root_y, win_x, win_y;
            unsigned int mask;
            if (XQueryPointer(display, root, &ret_root, &ret_child,
                              &root_x, &root_y, &win_x, &win_y, &mask)) {
                // Delta berekenen
                int dx = x - root_x;
                int dy = y - root_y;

                // Relatieve beweging uitvoeren
                XTestFakeRelativeMotionEvent(display, dx, dy, 0);
                XFlush(display);

                              }

            // Nieuwe positie loggen
            if (XQueryPointer(display, root, &ret_root, &ret_child,
                              &root_x, &root_y, &win_x, &win_y, &mask)) {
                // silence is golden
                              }
            // #else
            //             XTestFakeMotionEvent(display, 0, 10, 10, 0);
            //             XFlush(display);
            // #endif
        } catch (...) {
            XCloseDisplay(display);
            throw;
        }
        XCloseDisplay(display);
    }

    void Linux::LeftClick() {
        ClickButton(Button1);
    }

    void Linux::RightClick() {
        ClickButton(Button3);
    }

    void Linux::MoveAndLeftClick(const int x, const int y) {
        Display* display = GetDisplay();
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

    void Linux::MoveAndRightClick(const int x, const int y) {
        Display* display = GetDisplay();
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

    void Linux::Scroll(const int amount, int* x, int* y) {
        Display* display = GetDisplay();
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

    void Linux::ScrollUp(const int amount) {
        Scroll(amount, nullptr, nullptr);
    }

    void Linux::ScrollDown(const int amount) {
        Scroll(-amount, nullptr, nullptr);
    }
}
#endif