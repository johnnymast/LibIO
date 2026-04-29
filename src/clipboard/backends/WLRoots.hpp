#pragma once

#include "LibIO/ClipboardControls.hpp"

#if PLATFORM_WLROOTS
#include <wayland-client.h>
#include "data-control-unstable-v1-protocol.h"
#include <string>
#include <mutex>

namespace LibIO::Clipboard::Backends {
    class WLRoots : public ClipboardControls {
    public:
        ~WLRoots() override;

        static ClipboardControls &getInstance();

        void Copy(std::string text) override;

        std::string Paste() override;

        void Clear() override;

    private:
        WLRoots();

        wl_display *display = nullptr;
        wl_registry *registry = nullptr;
        wl_seat *seat = nullptr;

        zwlr_data_control_manager_v1 *manager = nullptr;
        zwlr_data_control_device_v1 *device = nullptr;
        zwlr_data_control_source_v1 *source = nullptr;

        std::string lastClipboard;
        std::mutex clipboardMutex;

        static void registryHandler(void *, wl_registry *, uint32_t, const char *, uint32_t);

        static void registryRemover(void *, wl_registry *, uint32_t);

        static void offerHandle(void *, zwlr_data_control_offer_v1 *, const char *);

        static void dataDeviceDataOffer(void *, zwlr_data_control_device_v1 *, zwlr_data_control_offer_v1 *);

        static void dataDeviceSelection(void *, zwlr_data_control_device_v1 *, zwlr_data_control_offer_v1 *);

        static void dataDevicePrimarySelection(void *, zwlr_data_control_device_v1 *, zwlr_data_control_offer_v1 *);
    };
}

#endif
