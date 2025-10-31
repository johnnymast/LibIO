#include "LibIO/keyboard/Windows.hpp"

#if PLATFORM_WINDOWS
#include <windows.h>

#include <Windows.h>
#include <unordered_map>
#include <string>
#include <thread>
#include <stdexcept>
#include <cctype>
#include <algorithm>

namespace LibIO::Keyboard {
    static void Windows::Initialize() {
        KeyCodes = {
            // Function keys
            {"f1", 0x70}, {"f2", 0x71}, {"f3", 0x72}, {"f4", 0x73},
            {"f5", 0x74}, {"f6", 0x75}, {"f7", 0x76}, {"f8", 0x77},
            {"f9", 0x78}, {"f10", 0x79}, {"f11", 0x7A}, {"f12", 0x7B},

            // Special keys
            {"enter", 0x0D}, {"esc", 0x1B}, {"escape", 0x1B},
            {"tab", 0x09}, {"space", 0x20}, {"backspace", 0x08},
            {"delete", 0x2E}, {"insert", 0x2D}, {"home", 0x24},
            {"end", 0x23}, {"pageup", 0x21}, {"pagedown", 0x22},

            // Arrow keys
            {"left", 0x25}, {"up", 0x26}, {"right", 0x27}, {"down", 0x28},

            // Modifier keys
            {"shift", 0x10}, {"ctrl", 0x11}, {"control", 0x11},
            {"alt", 0x12}, {"win", 0x5B}, {"windows", 0x5B},

            // Lock keys
            {"capslock", 0x14}, {"numlock", 0x90}, {"scrolllock", 0x91},

            // Punctuation
            {",", 0xBC}, {".", 0xBE}, {"/", 0xBF}, {";", 0xBA},
            {"'", 0xDE}, {"[", 0xDB}, {"]", 0xDD}, {"-", 0xBD},
            {"=", 0xBB}, {"`", 0xC0}, {"\\", 0xDC},

            // Numbers
            {"0", 0x30}, {"1", 0x31}, {"2", 0x32}, {"3", 0x33},
            {"4", 0x34}, {"5", 0x35}, {"6", 0x36}, {"7", 0x37},
            {"8", 0x38}, {"9", 0x39},

            // Letters
            {"a", 0x41}, {"b", 0x42}, {"c", 0x43}, {"d", 0x44},
            {"e", 0x45}, {"f", 0x46}, {"g", 0x47}, {"h", 0x48},
            {"i", 0x49}, {"j", 0x4A}, {"k", 0x4B}, {"l", 0x4C},
            {"m", 0x4D}, {"n", 0x4E}, {"o", 0x4F}, {"p", 0x50},
            {"q", 0x51}, {"r", 0x52}, {"s", 0x53}, {"t", 0x54},
            {"u", 0x55}, {"v", 0x56}, {"w", 0x57}, {"x", 0x58},
            {"y", 0x59}, {"z", 0x5A}
        };
    }

    Windows& Windows::getInstance() {
        static Windows instance;
        return instance;
    }

    // Convert a string to lowercase
    static std::string ToLower(const std::string &input) {
        std::string result = input;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    // Press a key by name
    static void PressKey(const std::string &key) {
        std::string lowerKey = ToLower(key);

        BYTE virtualKey = 0;
        bool keyFound = false;

        // Check if the key is in our predefined map
        auto it = KeyCodes.find(lowerKey);
        if (it != KeyCodes.end()) {
            virtualKey = it->second;
            keyFound = true;
        }
        // Handle single character keys
        else if (key.length() == 1) {
            SHORT vk = VkKeyScanA(key[0]);
            virtualKey = static_cast<BYTE>(vk & 0xFF);
            bool shift = (vk >> 8 & 1) != 0;

            if (shift) {
                keybd_event(0x10, 0, 0, 0); // SHIFT down
            }

            keybd_event(virtualKey, 0, 0, 0);
            keybd_event(virtualKey, 0, KEYEVENTF_KEYUP, 0);

            if (shift) {
                keybd_event(0x10, 0, KEYEVENTF_KEYUP, 0); // SHIFT up
            }

            keyFound = true;
        }

        // If we found the key in the map, press it
        if (keyFound && virtualKey != 0) {
            keybd_event(virtualKey, 0, 0, 0);
            keybd_event(virtualKey, 0, KEYEVENTF_KEYUP, 0);
        }
    }

    // Press a modifier key and another key together (hotkey)
    static void Hotkey(const std::string &modifier, const std::string &key) {
        std::string lowerModifier = ToLower(modifier);

        // Translate modifier string to virtual key code
        BYTE modifierKey;
        if (lowerModifier == "shift") {
            modifierKey = 0x10;
        } else if (lowerModifier == "ctrl" || lowerModifier == "control") {
            modifierKey = 0x11;
        } else if (lowerModifier == "alt") {
            modifierKey = 0x12;
        } else {
            throw std::invalid_argument("Unsupported modifier key: " + modifier);
        }

        // Find the virtual key code for the key
        BYTE virtualKey = 0;
        std::string lowerKey = ToLower(key);

        auto it = KeyCodes.find(lowerKey);
        if (it != KeyCodes.end()) {
            virtualKey = it->second;
        } else if (key.length() == 1) {
            SHORT vk = VkKeyScanA(key[0]);
            virtualKey = static_cast<BYTE>(vk & 0xFF);
        } else {
            throw std::invalid_argument("Unsupported key: " + key);
        }

        // Press the key combination
        keybd_event(modifierKey, 0, 0, 0); // Press modifier
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Small delay
        keybd_event(virtualKey, 0, 0, 0); // Press key
        keybd_event(virtualKey, 0, KEYEVENTF_KEYUP, 0); // Release key
        keybd_event(modifierKey, 0, KEYEVENTF_KEYUP, 0); // Release modifier
    }
}
#endif