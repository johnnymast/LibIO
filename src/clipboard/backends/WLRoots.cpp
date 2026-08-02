#include "WLRoots.hpp"

#if WLROOTS_CLIPBOARD
#include <cstring>
#include <unistd.h>

namespace LibIO::Clipboard::Backends {
    ClipboardControls &WLRoots::getInstance() {
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

        if (manager && seat && !device) {
            device = zwlr_data_control_manager_v1_get_data_device(manager, seat);

            static const zwlr_data_control_device_v1_listener devListener = {
                .data_offer = dataDeviceDataOffer,
                .selection = dataDeviceSelection,
                .primary_selection = dataDevicePrimarySelection
            };

            zwlr_data_control_device_v1_add_listener(device, &devListener, this);
            wl_display_roundtrip(display);
        }
    }

    WLRoots::~WLRoots() {
        if (device)
            zwlr_data_control_device_v1_destroy(device);
        if (manager)
            zwlr_data_control_manager_v1_destroy(manager);
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

        if (strcmp(interface, zwlr_data_control_manager_v1_interface.name) == 0)
            self->manager = static_cast<zwlr_data_control_manager_v1 *>(
                wl_registry_bind(reg, name, &zwlr_data_control_manager_v1_interface, 2)
            );
    }

    void WLRoots::registryRemover(void *, wl_registry *, uint32_t) {
    }

    void WLRoots::offerHandle(void *data, zwlr_data_control_offer_v1 *offer, const char *mime) {
        auto *self = static_cast<WLRoots *>(data);
        (void) self;
        (void) offer;
        (void) mime;
    }

    void WLRoots::dataDeviceDataOffer(void *data, zwlr_data_control_device_v1 *, zwlr_data_control_offer_v1 *offer) {
        static const zwlr_data_control_offer_v1_listener offerListener = {
            .offer = offerHandle
        };

        zwlr_data_control_offer_v1_add_listener(offer, &offerListener, data);
    }

    void WLRoots::dataDeviceSelection(void *data, zwlr_data_control_device_v1 *, zwlr_data_control_offer_v1 *offer) {
        auto *self = static_cast<WLRoots *>(data);
        if (!offer)
            return;
        (void) self;
    }

    void WLRoots::dataDevicePrimarySelection(void *, zwlr_data_control_device_v1 *, zwlr_data_control_offer_v1 *) {
    }

    void WLRoots::DoCopy(std::string text) {
        if (!manager || !device)
            return;

        {
            std::lock_guard<std::mutex> lock(clipboardMutex);
            lastClipboard = std::move(text);
        }

        source = zwlr_data_control_manager_v1_create_data_source(manager);
        zwlr_data_control_source_v1_offer(source, "text/plain;charset=utf-8");

        static const zwlr_data_control_source_v1_listener srcListener = {
            .send = [](void *data, zwlr_data_control_source_v1 *, const char *, int fd) {
                auto *self = static_cast<WLRoots *>(data);
                std::string copy;
                {
                    std::lock_guard<std::mutex> lock(self->clipboardMutex);
                    copy = self->lastClipboard;
                }
                write(fd, copy.c_str(), copy.size());
                close(fd);
            },
            .cancelled = [](void *, zwlr_data_control_source_v1 *) {
            }
        };

        zwlr_data_control_source_v1_add_listener(source, &srcListener, this);
        zwlr_data_control_device_v1_set_selection(device, source);

        wl_display_flush(display);
    }

    std::string WLRoots::DoPaste() {
        std::lock_guard<std::mutex> lock(clipboardMutex);
        return lastClipboard;
    }

    void WLRoots::DoClear() {
        std::lock_guard<std::mutex> lock(clipboardMutex);
        lastClipboard.clear();
    }
}

#endif
