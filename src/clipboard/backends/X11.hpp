#pragma once
#include "LibIO/ClipboardControls.hpp"

#if PLATFORM_X11
#include <X11/Xlib.h>

#include <string>
#include <thread>
#include <atomic>
#include <mutex>

namespace LibIO::Clipboard::Backends {

    class X11 : public ClipboardControls {
    public:

        ~X11() override;

        static ClipboardControls &getInstance();

        void Copy(std::string text) override;
        std::string Paste() override;
        void Clear() override;



    private:
        void clipboardEventLoop();
        void initX11();

        std::thread eventThread;
        std::atomic<bool> running{false};

        Display *display = nullptr;
        Window window = 0;
        Atom clipboardAtom = 0, targetsAtom = 0, utf8Atom = 0, stringAtom = 0, textAtom = 0;

        std::once_flag initOnce;
        bool initialized = false;

        std::mutex clipboardMutex;
        std::string clipboardContent;
    };
}
#endif