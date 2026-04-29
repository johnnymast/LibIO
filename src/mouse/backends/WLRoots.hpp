#pragma once

#include "LibIO/MouseControls.hpp"

#if PLATFORM_WLROOTS

#include <wayland-client.h>
#include "virtual-pointer-unstable-v1-protocol.h"

namespace LibIO::Mouse::Backends {

    class WLRoots : public MouseControls {
    public:
        static MouseControls& getInstance();
        ~WLRoots() override;

        void ClickButton(int button) override;
        void MoveCursor(int x, int y) override;
        void LeftClick() override;
        void RightClick() override;
        void MoveAndLeftClick(int x, int y) override;
        void MoveAndRightClick(int x, int y) override;
        void Scroll(int amount, int* x, int* y) override;
        void ScrollUp(int amount) override;
        void ScrollDown(int amount) override;

    private:
        WLRoots();

        wl_display* display = nullptr;
        wl_registry* registry = nullptr;
        wl_seat* seat = nullptr;

        zwlr_virtual_pointer_manager_v1* pointerManager = nullptr;
        zwlr_virtual_pointer_v1* pointer = nullptr;

        static void registryHandler(void* data, wl_registry* registry, uint32_t name, const char* interface, uint32_t version);
        static void registryRemover(void* data, wl_registry* registry, uint32_t name);
    };

}

#endif
