#include "LibIO/keyboard/Linux.hpp"

#if PLATFORM_LINUX

#include <algorithm>
#include <stdexcept>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <unordered_map>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace LibIO::Keyboard {
    // Other member declarations...
    std::unordered_map<char, std::pair<std::string, std::string> > Linux::SpecialCharacterMap = {
        {'@', {"shift", "2"}},
        {'#', {"shift", "3"}},
        {'$', {"shift", "4"}},
        {'%', {"shift", "5"}},
        {'^', {"shift", "6"}},
        {'&', {"shift", "7"}},
        {'*', {"shift", "8"}},
        {'(', {"shift", "9"}},
        {')', {"shift", "0"}},
        {'_', {"shift", "-"}},
        {'+', {"shift", "="}},
        {'{', {"shift", "["}},
        {'}', {"shift", "]"}},
        {'|', {"shift", "\\"}},
        {':', {"shift", ";"}},
        {'"', {"shift", "'"}},
        {'<', {"shift", ","}},
        {'>', {"shift", "."}},
        {'?', {"shift", "/"}},
        {'!', {"shift", "1"}},
        {'~', {"shift", "`"}}
    };

    std::unordered_map<std::string, uint> Linux::KeyCodes = {
        // Function keys
        {"f1", XK_F1}, {"f2", XK_F2}, {"f3", XK_F3}, {"f4", XK_F4},
        {"f5", XK_F5}, {"f6", XK_F6}, {"f7", XK_F7}, {"f8", XK_F8},
        {"f9", XK_F9}, {"f10", XK_F10}, {"f11", XK_F11}, {"f12", XK_F12},

        // Special keys
        {"enter", XK_Return}, {"esc", XK_Escape}, {"escape", XK_Escape},
        {"tab", XK_Tab}, {"space", XK_space}, {"backspace", XK_BackSpace},
        {"delete", XK_Delete}, {"insert", XK_Insert}, {"home", XK_Home},
        {"end", XK_End}, {"pageup", XK_Page_Up}, {"pagedown", XK_Page_Down},

        // Arrow keys
        {"left", XK_Left}, {"up", XK_Up}, {"right", XK_Right}, {"down", XK_Down},

        // Modifier keys
        {"shift", XK_Shift_L}, {"ctrl", XK_Control_L}, {"control", XK_Control_L},
        {"alt", XK_Alt_L}, {"win", XK_Super_L}, {"windows", XK_Super_L},

        // Lock keys
        {"capslock", XK_Caps_Lock}, {"numlock", XK_Num_Lock}, {"scrolllock", XK_Scroll_Lock},

        // Basic characters
        {",", XK_comma}, {".", XK_period}, {"/", XK_slash}, {";", XK_semicolon},
        {"'", XK_apostrophe}, {"[", XK_bracketleft}, {"]", XK_bracketright}, {"-", XK_minus},
        {"=", XK_equal}, {"`", XK_grave}, {"\\", XK_backslash},

        // Numbers
        {"0", XK_0}, {"1", XK_1}, {"2", XK_2}, {"3", XK_3},
        {"4", XK_4}, {"5", XK_5}, {"6", XK_6}, {"7", XK_7},
        {"8", XK_8}, {"9", XK_9},

        // Letters
        {"a", XK_a}, {"b", XK_b}, {"c", XK_c}, {"d", XK_d},
        {"e", XK_e}, {"f", XK_f}, {"g", XK_g}, {"h", XK_h},
        {"i", XK_i}, {"j", XK_j}, {"k", XK_k}, {"l", XK_l},
        {"m", XK_m}, {"n", XK_n}, {"o", XK_o}, {"p", XK_p},
        {"q", XK_q}, {"r", XK_r}, {"s", XK_s}, {"t", XK_t},
        {"u", XK_u}, {"v", XK_v}, {"w", XK_w}, {"x", XK_x},
        {"y", XK_y}, {"z", XK_z},

        {" ", XK_space}
    };


    Display *Linux::GetDisplay() {
        Display *display = XOpenDisplay(nullptr);
        if (!display) {
            throw std::runtime_error("Could not open X11 display");
        }
        return display;
    }

    std::string Linux::ToLower(const std::string &input) {
        std::string result = input;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    void Linux::PressKey(const std::string &key) {
        if (key.empty()) return;

        if (key == "\n") {
            PressKey("enter");
            return;
        }

        if (key.length() == 1) {
            char c = key[0];
            auto it = SpecialCharacterMap.find(c);
            if (it != SpecialCharacterMap.end()) {
                Hotkey(it->second.first, it->second.second);
                return;
            }
        }

        Display *display = GetDisplay();
        try {
            std::string lowerKey = ToLower(key);
            auto it = KeyCodes.find(lowerKey);
            KeySym keySym = 0;

            if (it != KeyCodes.end()) {
                keySym = it->second;
            } else if (key.length() == 1) {
                char c = key[0];
                if (std::isupper(c)) {
                    Hotkey("shift", std::string(1, std::tolower(c)));
                    return;
                }
                keySym = c;
            } else {
                XCloseDisplay(display);
                throw std::invalid_argument("Unsupported key: " + key);
            }

            KeyCode keyCode = XKeysymToKeycode(display, keySym);
            if (keyCode == 0) {
                XCloseDisplay(display);
                throw std::invalid_argument("Could not map key to keycode: " + key);
            }

            XTestFakeKeyEvent(display, keyCode, True, 0);
            XTestFakeKeyEvent(display, keyCode, False, 0);
            XFlush(display);
            XCloseDisplay(display);

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        } catch (...) {
            XCloseDisplay(display);
            throw;
        }
    }

    void Linux::Hotkey(const std::string &modifier, const std::string &key) {
        Display *display = GetDisplay();
        try {
            std::vector<std::string> modifiers;
            std::string mod = modifier;
            size_t pos = 0;
            while ((pos = mod.find('+')) != std::string::npos) {
                modifiers.push_back(mod.substr(0, pos));
                mod.erase(0, pos + 1);
            }
            if (!mod.empty()) {
                modifiers.push_back(mod);
            }

            std::vector<KeyCode> modifierCodes;

            for (const auto &m: modifiers) {
                std::string lowerMod = ToLower(m);
                auto it = KeyCodes.find(lowerMod);
                if (it == KeyCodes.end()) {
                    XCloseDisplay(display);
                    throw std::invalid_argument("Unsupported modifier key: " + m);
                }

                KeySym modifierSym = it->second;
                KeyCode modifierCode = XKeysymToKeycode(display, modifierSym);
                if (modifierCode == 0) {
                    XCloseDisplay(display);
                    throw std::invalid_argument("Could not map modifier to keycode: " + m);
                }

                modifierCodes.push_back(modifierCode);
            }

            std::string lowerKey = ToLower(key);
            KeySym keySym = 0;
            auto it = KeyCodes.find(lowerKey);

            if (it != KeyCodes.end()) {
                keySym = it->second;
            } else if (key.length() == 1) {
                keySym = key[0];
            } else {
                XCloseDisplay(display);
                throw std::invalid_argument("Unsupported key: " + key);
            }

            KeyCode keyCode = XKeysymToKeycode(display, keySym);
            if (keyCode == 0) {
                XCloseDisplay(display);
                throw std::invalid_argument("Could not map key to keycode: " + key);
            }

            for (const auto &modCode: modifierCodes) {
                XTestFakeKeyEvent(display, modCode, True, 0);
                XFlush(display);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            XTestFakeKeyEvent(display, keyCode, True, 0);
            XFlush(display);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            XTestFakeKeyEvent(display, keyCode, False, 0);
            XFlush(display);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            for (auto it = modifierCodes.rbegin(); it != modifierCodes.rend(); ++it) {
                XTestFakeKeyEvent(display, *it, False, 0);
                XFlush(display);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            XFlush(display);
            XCloseDisplay(display);

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        } catch (...) {
            XCloseDisplay(display);
            throw;
        }
    }
}
#endif
