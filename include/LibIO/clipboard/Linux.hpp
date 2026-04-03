#pragma once

#include "ClipboardControls.hpp"

#if PLATFORM_LINUX
#include <X11/Xlib.h>

#include <string>
#include <thread>
#include <atomic>
#include <mutex>

namespace LibIO::Clipboard {

    class Linux : public ClipboardControls {
    public:
        static ClipboardControls &getInstance();

        void Copy(std::string text) override;
        std::string Paste() override;
        void Clear() override;

        ~Linux();

    private:
        void clipboardEventLoop();
        void initX11();

        // Event-loop control
        std::thread eventThread;
        std::atomic<bool> running{false};

        // X11 state (instance members, no static inline)
        Display *display = nullptr;
        Window window = 0;
        Atom clipboardAtom = 0, targetsAtom = 0, utf8Atom = 0, stringAtom = 0, textAtom = 0;

        // Init guard
        std::once_flag initOnce;
        bool initialized = false;

        // Clipboard data (instance-scoped)
        std::mutex clipboardMutex;
        std::string clipboardContent;
    };

} // namespace LibIO::Clipboard
#endif
