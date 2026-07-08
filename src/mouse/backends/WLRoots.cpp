#include "WLRoots.hpp"

#if PLATFORM_WLROOTS
#include <linux/input-event-codes.h>
#include <cstring>
#include <chrono>
#include <iostream>
#include <thread>

namespace LibIO::Mouse::Backends {

    static uint32_t timestamp_ms() {
        using namespace std::chrono;
        return (uint32_t) duration_cast<milliseconds>(
            steady_clock::now().time_since_epoch()
        ).count();
    }


    MouseControls &WLRoots::getInstance() {
        static WLRoots instance;
        return instance;
    }

    WLRoots::WLRoots() {
        display = wl_display_connect(nullptr);
        if (!display)
            return;

        registry = wl_display_get_registry(display);

        static const wl_registry_listener listener = {
            .global = registryHandler,
            .global_remove = registryRemover
        };

        wl_registry_add_listener(registry, &listener, this);
        wl_display_roundtrip(display);

        if (pointerManager && seat)
            pointer = zwlr_virtual_pointer_manager_v1_create_virtual_pointer(pointerManager, seat);
    }

    WLRoots::~WLRoots() {
        if (pointer)
            zwlr_virtual_pointer_v1_destroy(pointer);
        if (pointerManager)
            zwlr_virtual_pointer_manager_v1_destroy(pointerManager);
        if (seat)
            wl_seat_destroy(seat);
        if (registry)
            wl_registry_destroy(registry);
        if (display)
            wl_display_disconnect(display);
    }

    void WLRoots::registryHandler(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t) {
        auto *self = static_cast<WLRoots *>(data);

        if (strcmp(interface, wl_seat_interface.name) == 0) {
            self->seat = static_cast<wl_seat *>(
                wl_registry_bind(registry, name, &wl_seat_interface, 1)
            );
        } else if (strcmp(interface, zwlr_virtual_pointer_manager_v1_interface.name) == 0) {
            self->pointerManager = static_cast<zwlr_virtual_pointer_manager_v1 *>(
                wl_registry_bind(registry, name, &zwlr_virtual_pointer_manager_v1_interface, 1)
            );
        }
    }

    void WLRoots::registryRemover(void *, wl_registry *, uint32_t) {
    }

    void WLRoots::MoveCursor(int x, int y) {
        if (!pointer || !display)
            return;

        zwlr_virtual_pointer_v1_motion_absolute(
            pointer,
            0,
            x,
            y,
            1920,
            1080
        );

        zwlr_virtual_pointer_v1_frame(pointer);
        wl_display_flush(display);
    }

    void WLRoots::ClickButton(int button) {
        if (!pointer || !display)
            return;

        zwlr_virtual_pointer_v1_button(
            pointer,
            0,
            button,
            WL_POINTER_BUTTON_STATE_PRESSED
        );
        zwlr_virtual_pointer_v1_frame(pointer);
        wl_display_flush(display);

        zwlr_virtual_pointer_v1_button(
            pointer,
            10,
            button,
            WL_POINTER_BUTTON_STATE_RELEASED
        );
        zwlr_virtual_pointer_v1_frame(pointer);
        wl_display_flush(display);
    }

    void WLRoots::LeftClick() {
        ClickButton(BTN_LEFT);
    }

    void WLRoots::RightClick() {
        ClickButton(BTN_RIGHT);
    }

    void WLRoots::MoveAndLeftClick(int x, int y) {
        MoveCursor(x, y);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        LeftClick();
    }

    void WLRoots::MoveAndRightClick(int x, int y) {
        MoveCursor(x, y);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        RightClick();
    }

    void WLRoots::Scroll(int amount, int*, int*) {
        if (!pointer || !display)
            return;

        int scaled = amount * 120;
        uint32_t ts = timestamp_ms();

        // Pixel scroll
        zwlr_virtual_pointer_v1_axis(
            pointer,
            ts,
            0,
            wl_fixed_from_int(scaled)
        );

        // Discrete scroll (jouw wlroots verwacht 5 parameters!)
        zwlr_virtual_pointer_v1_axis_discrete(
            pointer,
            ts,
            0,
            wl_fixed_from_int(scaled),
            amount
        );

        zwlr_virtual_pointer_v1_frame(pointer);
        wl_display_flush(display);
    }

    void WLRoots::ScrollUp(int amount) {
        Scroll(-amount, nullptr, nullptr);
    }

    void WLRoots::ScrollDown(int amount) {
        Scroll(amount, nullptr, nullptr);
    }
}

#endif
