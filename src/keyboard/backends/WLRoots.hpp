#pragma once

#include "LibIO/KeyboardControls.hpp"

#if PLATFORM_WLROOTS
#include <wayland-client.h>
#include "virtual-keyboard-unstable-v1-protocol.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace LibIO::Keyboard::Backends {

    class WLRoots : public KeyboardControls {
    public:
        ~WLRoots() override;
        static KeyboardControls& getInstance();

        void PressKey(const std::string& key) override;
        void Hotkey(const std::string& modifier, const std::string& key) override;

    private:
        WLRoots();

        wl_display* display = nullptr;
        wl_registry* registry = nullptr;
        wl_seat* seat = nullptr;

        zwp_virtual_keyboard_manager_v1* vk_manager = nullptr;
        zwp_virtual_keyboard_v1* vk = nullptr;

        static void registryHandler(void*, wl_registry*, uint32_t, const char*, uint32_t);
        static void registryRemover(void*, wl_registry*, uint32_t);

        static int create_anonymous_file(off_t size);
        void setup_keymap();

        static std::string ToLower(const std::string& s);

        static std::unordered_map<char, std::pair<std::string, std::string>> SpecialCharacterMap;
        static std::unordered_map<std::string, uint32_t> KeyCodes;
    };

}

#endif
