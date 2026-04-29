#include "WLRoots.hpp"

#if PLATFORM_WLROOTS
#include <linux/input-event-codes.h>
#include <xkbcommon/xkbcommon.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <cstring>
#include <algorithm>

namespace LibIO::Keyboard::Backends {
    std::unordered_map<char, std::pair<std::string, std::string> > WLRoots::SpecialCharacterMap = {
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

    std::unordered_map<std::string, uint32_t> WLRoots::KeyCodes = {
        {"f1", KEY_F1}, {"f2", KEY_F2}, {"f3", KEY_F3}, {"f4", KEY_F4},
        {"f5", KEY_F5}, {"f6", KEY_F6}, {"f7", KEY_F7}, {"f8", KEY_F8},
        {"f9", KEY_F9}, {"f10", KEY_F10}, {"f11", KEY_F11}, {"f12", KEY_F12},

        {"enter", KEY_ENTER}, {"esc", KEY_ESC}, {"escape", KEY_ESC},
        {"tab", KEY_TAB}, {"space", KEY_SPACE}, {"backspace", KEY_BACKSPACE},
        {"delete", KEY_DELETE}, {"insert", KEY_INSERT}, {"home", KEY_HOME},
        {"end", KEY_END}, {"pageup", KEY_PAGEUP}, {"pagedown", KEY_PAGEDOWN},

        {"left", KEY_LEFT}, {"up", KEY_UP}, {"right", KEY_RIGHT}, {"down", KEY_DOWN},

        {"shift", KEY_LEFTSHIFT}, {"ctrl", KEY_LEFTCTRL}, {"control", KEY_LEFTCTRL},
        {"alt", KEY_LEFTALT}, {"win", KEY_LEFTMETA}, {"windows", KEY_LEFTMETA},

        {"capslock", KEY_CAPSLOCK}, {"numlock", KEY_NUMLOCK}, {"scrolllock", KEY_SCROLLLOCK},

        {",", KEY_COMMA}, {".", KEY_DOT}, {"/", KEY_SLASH}, {";", KEY_SEMICOLON},
        {"'", KEY_APOSTROPHE}, {"[", KEY_LEFTBRACE}, {"]", KEY_RIGHTBRACE}, {"-", KEY_MINUS},
        {"=", KEY_EQUAL}, {"`", KEY_GRAVE}, {"\\", KEY_BACKSLASH},

        {"0", KEY_0}, {"1", KEY_1}, {"2", KEY_2}, {"3", KEY_3},
        {"4", KEY_4}, {"5", KEY_5}, {"6", KEY_6}, {"7", KEY_7},
        {"8", KEY_8}, {"9", KEY_9},

        {"a", KEY_A}, {"b", KEY_B}, {"c", KEY_C}, {"d", KEY_D},
        {"e", KEY_E}, {"f", KEY_F}, {"g", KEY_G}, {"h", KEY_H},
        {"i", KEY_I}, {"j", KEY_J}, {"k", KEY_K}, {"l", KEY_L},
        {"m", KEY_M}, {"n", KEY_N}, {"o", KEY_O}, {"p", KEY_P},
        {"q", KEY_Q}, {"r", KEY_R}, {"s", KEY_S}, {"t", KEY_T},
        {"u", KEY_U}, {"v", KEY_V}, {"w", KEY_W}, {"x", KEY_X},
        {"y", KEY_Y}, {"z", KEY_Z},

        {" ", KEY_SPACE}
    };

    KeyboardControls &WLRoots::getInstance() {
        static WLRoots instance;
        return instance;
    }

    WLRoots::WLRoots() {
        display = wl_display_connect(nullptr);
        if (!display)
            return;

        registry = wl_display_get_registry(display);
        if (!registry)
            return;

        static const wl_registry_listener regListener = {
            .global = registryHandler,
            .global_remove = registryRemover
        };

        wl_registry_add_listener(registry, &regListener, this);
        wl_display_roundtrip(display);

        if (vk_manager && seat) {
            vk = zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(vk_manager, seat);
            setup_keymap();
        }
    }

    WLRoots::~WLRoots() {
        if (vk)
            zwp_virtual_keyboard_v1_destroy(vk);
        if (vk_manager)
            zwp_virtual_keyboard_manager_v1_destroy(vk_manager);
        if (seat)
            wl_seat_destroy(seat);
        if (registry)
            wl_registry_destroy(registry);
        if (display)
            wl_display_disconnect(display);
    }

    void WLRoots::registryHandler(void *data, wl_registry *reg, uint32_t name, const char *interface,
                                  uint32_t version) {
        auto *self = static_cast<WLRoots *>(data);

        if (strcmp(interface, wl_seat_interface.name) == 0)
            self->seat = static_cast<wl_seat *>(wl_registry_bind(reg, name, &wl_seat_interface, 1));

        if (strcmp(interface, zwp_virtual_keyboard_manager_v1_interface.name) == 0)
            self->vk_manager = static_cast<zwp_virtual_keyboard_manager_v1 *>(
                wl_registry_bind(reg, name, &zwp_virtual_keyboard_manager_v1_interface, 1)
            );
    }

    void WLRoots::registryRemover(void *, wl_registry *, uint32_t) {
    }

    int WLRoots::create_anonymous_file(off_t size) {
        int fd = syscall(SYS_memfd_create, "vk-keymap", MFD_CLOEXEC);
        if (fd < 0)
            return -1;
        if (ftruncate(fd, size) < 0) {
            close(fd);
            return -1;
        }
        return fd;
    }

    void WLRoots::setup_keymap() {
        xkb_context *ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
        if (!ctx)
            return;

        xkb_rule_names names = {};
        names.rules = "evdev";
        names.model = "pc105";
        names.layout = "us";

        xkb_keymap *keymap = xkb_keymap_new_from_names(ctx, &names, XKB_KEYMAP_COMPILE_NO_FLAGS);
        if (!keymap) {
            xkb_context_unref(ctx);
            return;
        }

        char *keymap_str = xkb_keymap_get_as_string(keymap, XKB_KEYMAP_FORMAT_TEXT_V1);
        if (!keymap_str) {
            xkb_keymap_unref(keymap);
            xkb_context_unref(ctx);
            return;
        }

        size_t size = strlen(keymap_str) + 1;
        int fd = create_anonymous_file(size);
        if (fd < 0) {
            free(keymap_str);
            xkb_keymap_unref(keymap);
            xkb_context_unref(ctx);
            return;
        }

        void *map = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (map == MAP_FAILED) {
            close(fd);
            free(keymap_str);
            xkb_keymap_unref(keymap);
            xkb_context_unref(ctx);
            return;
        }

        memcpy(map, keymap_str, size);
        munmap(map, size);

        zwp_virtual_keyboard_v1_keymap(
            vk,
            XKB_KEYMAP_FORMAT_TEXT_V1,
            fd,
            size
        );

        close(fd);
        free(keymap_str);
        xkb_keymap_unref(keymap);
        xkb_context_unref(ctx);
    }

    std::string WLRoots::ToLower(const std::string &s) {
        std::string out = s;
        std::transform(out.begin(), out.end(), out.begin(), ::tolower);
        return out;
    }

    void WLRoots::PressKey(const std::string &key) {
        if (!vk || !display)
            return;

        if (key.empty())
            return;

        if (key == "\n") {
            PressKey("enter");
            return;
        }

        if (key.size() == 1) {
            char c = key[0];
            auto itSpec = SpecialCharacterMap.find(c);
            if (itSpec != SpecialCharacterMap.end()) {
                Hotkey(itSpec->second.first, itSpec->second.second);
                return;
            }
        }

        std::string lower = ToLower(key);
        uint32_t code = 0;

        auto it = KeyCodes.find(lower);
        if (it != KeyCodes.end()) {
            code = it->second;
        } else if (key.size() == 1) {
            char c = key[0];
            if (std::isupper(static_cast<unsigned char>(c))) {
                std::string base(1, static_cast<char>(std::tolower(c)));
                Hotkey("shift", base);
                return;
            }
            std::string base(1, c);
            auto it2 = KeyCodes.find(base);
            if (it2 != KeyCodes.end())
                code = it2->second;
        }

        if (!code)
            return;

        zwp_virtual_keyboard_v1_key(vk, 0, code, WL_KEYBOARD_KEY_STATE_PRESSED);
        zwp_virtual_keyboard_v1_key(vk, 10, code, WL_KEYBOARD_KEY_STATE_RELEASED);
        wl_display_flush(display);
    }

    void WLRoots::Hotkey(const std::string& modifier, const std::string& key) {
        if (!vk || !display)
            return;

        // Parse modifiers (supports "ctrl+shift" etc.)
        std::string mod = modifier;
        std::vector<std::string> modifiers;
        size_t pos = 0;
        while ((pos = mod.find('+')) != std::string::npos) {
            modifiers.push_back(mod.substr(0, pos));
            mod.erase(0, pos + 1);
        }
        if (!mod.empty())
            modifiers.push_back(mod);

        uint32_t depressedMask = 0;

        // Build modifier bitmask
        for (auto& m : modifiers) {
            std::string lower = ToLower(m);

            if (lower == "shift")
                depressedMask |= (1 << 0);   // shift
            else if (lower == "ctrl" || lower == "control")
                depressedMask |= (1 << 2);   // ctrl
            else if (lower == "alt")
                depressedMask |= (1 << 3);   // alt
        }

        // Lookup keycode
        std::string lowerKey = ToLower(key);
        uint32_t keyCode = 0;

        auto it = KeyCodes.find(lowerKey);
        if (it != KeyCodes.end())
            keyCode = it->second;
        else if (key.size() == 1) {
            std::string base(1, key[0]);
            auto it2 = KeyCodes.find(base);
            if (it2 != KeyCodes.end())
                keyCode = it2->second;
        }

        if (!keyCode)
            return;

        // Apply modifiers
        zwp_virtual_keyboard_v1_modifiers(vk, depressedMask, 0, 0, 0);
        wl_display_flush(display);

        // Press key
        zwp_virtual_keyboard_v1_key(vk, 0, keyCode, WL_KEYBOARD_KEY_STATE_PRESSED);
        wl_display_flush(display);

        // Release key
        zwp_virtual_keyboard_v1_key(vk, 10, keyCode, WL_KEYBOARD_KEY_STATE_RELEASED);
        wl_display_flush(display);

        // Clear modifiers
        zwp_virtual_keyboard_v1_modifiers(vk, 0, 0, 0, 0);
        wl_display_flush(display);
    }
}
#endif