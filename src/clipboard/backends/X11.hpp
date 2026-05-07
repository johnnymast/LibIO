#pragma once
#include "LibIO/ClipboardControls.hpp"

#if X11_CLIPBOARD
#include <X11/Xlib.h>

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>

namespace LibIO::Clipboard::Backends {

    class X11 : public ClipboardControls {
    public:
        X11();
        ~X11() override;

        static ClipboardControls &getInstance();

        void Copy(std::string text) override;
        std::string Paste() override;
        void Clear() override;

    private:
        // X11 thread
        void threadMain();

        // Job queue
        void enqueue(std::function<void()> fn);
        void pumpJobs();

        // X11 event handling
        void handleEvent(const XEvent &ev);

        // Wine/Proton sync helper
        void ForceWineSync();

        // Internal X11 state
        Display *display = nullptr;
        Window window = 0;

        Atom clipboardAtom = 0;
        Atom targetsAtom = 0;
        Atom utf8Atom = 0;
        Atom stringAtom = 0;
        Atom textAtom = 0;

        // Clipboard data
        std::mutex clipboardMutex;
        std::string clipboardContent;

        // Threading
        std::thread x11Thread;
        std::atomic<bool> running{false};

        std::mutex queueMutex;
        std::condition_variable queueCV;
        std::queue<std::function<void()>> jobQueue;

        bool initialized = false;
    };

}
#endif
