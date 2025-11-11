#include "LibIO/clipboard/Linux.hpp"
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>

#if PLATFORM_LINUX
namespace LibIO::Clipboard {

    ClipboardControls &Linux::getInstance() {
        static Linux instance;
        return instance;
    }

    void Linux::initX11() {
        std::call_once(initOnce, [&]() {
            // Prepare Xlib for multi-thread usage
            if (!XInitThreads()) {
                std::cerr << "Warning: XInitThreads() failed; Xlib may not be thread-safe.\n";
            }

            display = XOpenDisplay(nullptr);
            if (!display) {
                std::cerr << "Failed to open X11 display\n";
                return;
            }

            window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                                         0, 0, 1, 1, 0, 0, 0);

            clipboardAtom = XInternAtom(display, "CLIPBOARD", False);
            targetsAtom   = XInternAtom(display, "TARGETS", False);
            utf8Atom      = XInternAtom(display, "UTF8_STRING", False);
            stringAtom    = XInternAtom(display, "STRING", False);
            textAtom      = XInternAtom(display, "TEXT", False);

            initialized = true;
        });
    }

    void Linux::Copy(std::string text) {
        initX11();
        if (!initialized || !display || window == 0) return;

        {
            std::lock_guard<std::mutex> lock(clipboardMutex);
            // Use copy first; move can be restored once stable
            clipboardContent = text;
        }

        XSetSelectionOwner(display, clipboardAtom, window, CurrentTime);
        XFlush(display);

        bool expected = false;
        if (running.compare_exchange_strong(expected, true)) {
            eventThread = std::thread(&Linux::clipboardEventLoop, this);
        }
    }

    void Linux::clipboardEventLoop() {
        while (running) {
            if (!display) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            while (XPending(display)) {
                XEvent event;
                XNextEvent(display, &event);

                if (event.type == SelectionRequest) {
                    XSelectionRequestEvent *req = &event.xselectionrequest;
                    XEvent respond{};
                    respond.xselection.type      = SelectionNotify;
                    respond.xselection.display   = req->display;
                    respond.xselection.requestor = req->requestor;
                    respond.xselection.selection = req->selection;
                    respond.xselection.target    = req->target;
                    respond.xselection.time      = req->time;
                    respond.xselection.property  = None;

                    if (req->property == None) {
                        req->property = req->target;
                    }

                    if (req->target == targetsAtom) {
                        Atom typeList[] = { utf8Atom, stringAtom, textAtom };
                        XChangeProperty(display, req->requestor, req->property,
                                        XA_ATOM, 32, PropModeReplace,
                                        reinterpret_cast<unsigned char *>(typeList), 3);
                        respond.xselection.property = req->property;
                    } else if (req->target == utf8Atom || req->target == stringAtom || req->target == textAtom) {
                        std::lock_guard<std::mutex> lock(clipboardMutex);
                        const unsigned char* bytes =
                            reinterpret_cast<const unsigned char *>(clipboardContent.c_str());
                        int len = static_cast<int>(clipboardContent.size());

                        XChangeProperty(display, req->requestor, req->property,
                                        req->target, 8, PropModeReplace,
                                        bytes, len);
                        respond.xselection.property = req->property;
                    }

                    XSendEvent(display, req->requestor, False, 0, &respond);
                    XFlush(display);
                } else if (event.type == SelectionClear) {
                    // Another owner took the selection; you can decide behavior here if needed
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }

    std::string Linux::Paste() {
        initX11();
        if (!initialized || !display || window == 0) return "";

        // If we are the owner, return local buffer
        if (XGetSelectionOwner(display, clipboardAtom) == window) {
            std::lock_guard<std::mutex> lock(clipboardMutex);
            return clipboardContent;
        }

        Atom property = XInternAtom(display, "COPIED_TEXT", False);

        auto convertAndWait = [&](Atom target) -> std::string {
            XConvertSelection(display, clipboardAtom, target, property, window, CurrentTime);
            XSync(display, False);

            std::string result;
            auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);

            while (std::chrono::steady_clock::now() < deadline) {
                if (XPending(display)) {
                    XEvent event;
                    XNextEvent(display, &event);

                    if (event.type == SelectionNotify) {
                        if (event.xselection.property) {
                            Atom actualType;
                            int actualFormat;
                            unsigned long nItems, bytesAfter;
                            unsigned char *data = nullptr;

                            if (Success == XGetWindowProperty(display, window, event.xselection.property,
                                                              0, (~0L), True, AnyPropertyType,
                                                              &actualType, &actualFormat,
                                                              &nItems, &bytesAfter, &data)) {
                                if (data) {
                                    result.assign(reinterpret_cast<char *>(data), nItems);
                                    XFree(data);
                                }
                            }
                        }
                        break;
                    }
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }
            }
            return result;
        };

        std::string result = convertAndWait(utf8Atom);
        if (result.empty()) {
            result = convertAndWait(stringAtom);
        }
        return result;
    }

    void Linux::Clear() {
        initX11();
        if (!initialized || !display) return;

        {
            std::lock_guard<std::mutex> lock(clipboardMutex);
            clipboardContent.clear();
        }
        XSetSelectionOwner(display, clipboardAtom, None, CurrentTime);
        XFlush(display);
    }

    Linux::~Linux() {
        running = false;
        if (eventThread.joinable()) {
            eventThread.join();
        }
        // Optional: close display if you own lifecycle completely
        // if (display) { XCloseDisplay(display); display = nullptr; initialized = false; }
    }

} // namespace LibIO::Clipboard
#endif
