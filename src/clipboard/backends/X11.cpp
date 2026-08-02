#include "X11.hpp"

#if X11_CLIPBOARD
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <iostream>
#include <chrono>
#include <thread>

namespace LibIO::Clipboard::Backends {

    ClipboardControls &X11::getInstance() {
        static X11 instance;
        return instance;
    }

    // ------------------------------------------------------------
    // Constructor: start dedicated X11 thread
    // ------------------------------------------------------------
    X11::X11() {
        running = true;
        x11Thread = std::thread(&X11::threadMain, this);
    }

    // ------------------------------------------------------------
    // Destructor: stop thread
    // ------------------------------------------------------------
    X11::~X11() {
        running = false;
        queueCV.notify_all();
        if (x11Thread.joinable())
            x11Thread.join();
    }

    // ------------------------------------------------------------
    // Enqueue job for X11 thread
    // ------------------------------------------------------------
    void X11::enqueue(std::function<void()> fn) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            jobQueue.push(std::move(fn));
        }
        queueCV.notify_one();
    }

    // ------------------------------------------------------------
    // Execute queued jobs
    // ------------------------------------------------------------
    void X11::pumpJobs() {
        std::queue<std::function<void()>> local;

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            std::swap(local, jobQueue);
        }

        while (!local.empty()) {
            local.front()();
            local.pop();
        }
    }

    // ------------------------------------------------------------
    // Dedicated X11 thread
    // ------------------------------------------------------------
    void X11::threadMain() {
        display = XOpenDisplay(nullptr);
        if (!display) {
            std::cerr << "Failed to open X11 display\n";
            return;
        }

        window = XCreateSimpleWindow(
            display,
            DefaultRootWindow(display),
            0, 0, 1, 1, 0, 0, 0
        );

        clipboardAtom = XInternAtom(display, "CLIPBOARD", False);
        targetsAtom   = XInternAtom(display, "TARGETS", False);
        utf8Atom      = XInternAtom(display, "UTF8_STRING", False);
        stringAtom    = XInternAtom(display, "STRING", False);
        textAtom      = XInternAtom(display, "TEXT", False);

        initialized = true;

        while (running) {
            pumpJobs();

            while (XPending(display)) {
                XEvent ev;
                XNextEvent(display, &ev);
                handleEvent(ev);
            }

            std::unique_lock<std::mutex> lock(queueMutex);
            queueCV.wait_for(lock, std::chrono::milliseconds(5));
        }

        XCloseDisplay(display);
        display = nullptr;
    }

    // ------------------------------------------------------------
    // Handle SelectionRequest / SelectionClear
    // ------------------------------------------------------------
    void X11::handleEvent(const XEvent &event) {
        if (event.type != SelectionRequest)
            return;

        XSelectionRequestEvent *req = (XSelectionRequestEvent *)&event;

        XEvent respond{};
        respond.xselection.type      = SelectionNotify;
        respond.xselection.display   = req->display;
        respond.xselection.requestor = req->requestor;
        respond.xselection.selection = req->selection;
        respond.xselection.target    = req->target;
        respond.xselection.time      = req->time;
        respond.xselection.property  = None;

        Atom property = req->property;
        if (property == None)
            property = req->target;

        if (req->target == targetsAtom) {
            Atom types[] = {utf8Atom, stringAtom, textAtom};
            XChangeProperty(display, req->requestor, property,
                            XA_ATOM, 32, PropModeReplace,
                            reinterpret_cast<unsigned char*>(types), 3);
            respond.xselection.property = property;
        }
        else if (req->target == utf8Atom ||
                 req->target == stringAtom ||
                 req->target == textAtom) {

            std::lock_guard<std::mutex> lock(clipboardMutex);
            const unsigned char *bytes =
                reinterpret_cast<const unsigned char*>(clipboardContent.c_str());
            int len = clipboardContent.size();

            XChangeProperty(display, req->requestor, property,
                            req->target, 8, PropModeReplace,
                            bytes, len);
            respond.xselection.property = property;
        }

        XSendEvent(display, req->requestor, False, 0, &respond);
        XFlush(display);
    }

    // ------------------------------------------------------------
    // Force Wine/Proton to sync its internal clipboard with X11
    // ------------------------------------------------------------
    void X11::ForceWineSync() {
        if (!initialized || !display) return;

        Atom syncProp = XInternAtom(display, "WINE_SYNC", False);

        XConvertSelection(
            display,
            clipboardAtom,
            utf8Atom,
            syncProp,
            window,
            CurrentTime
        );
        XFlush(display);
    }

    // ------------------------------------------------------------
    // Public Copy() — runs on X11 thread
    // ------------------------------------------------------------
    void X11::DoCopy(std::string text) {
        enqueue([this, text = std::move(text)] {
            if (!initialized) return;

            {
                std::lock_guard<std::mutex> lock(clipboardMutex);
                clipboardContent = text;
            }

            XSetSelectionOwner(display, clipboardAtom, window, CurrentTime);
            XFlush(display);

            // 🔥 Forceer Wine/Proton om de X11 clipboard over te nemen
            ForceWineSync();
        });
    }

    // ------------------------------------------------------------
    // Public Paste() — uses isolated Display (safe)
    // ------------------------------------------------------------
    std::string X11::DoPaste() {
        Display *dpy = XOpenDisplay(nullptr);
        if (!dpy) return "";

        Window win = XCreateSimpleWindow(
            dpy,
            DefaultRootWindow(dpy),
            0, 0, 1, 1, 0, 0, 0
        );

        Atom clipboardAtom = XInternAtom(dpy, "CLIPBOARD", False);
        Atom utf8Atom      = XInternAtom(dpy, "UTF8_STRING", False);
        Atom stringAtom    = XInternAtom(dpy, "STRING", False);
        Atom propertyAtom  = XInternAtom(dpy, "COPIED_TEXT", False);

        auto convertAndWait = [&](Atom target) -> std::string {
            XConvertSelection(dpy, clipboardAtom, target, propertyAtom, win, CurrentTime);
            XFlush(dpy);

            std::string result;
            auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);

            while (std::chrono::steady_clock::now() < deadline) {
                if (XPending(dpy)) {
                    XEvent ev;
                    XNextEvent(dpy, &ev);

                    if (ev.type == SelectionNotify && ev.xselection.requestor == win) {
                        if (ev.xselection.property != None) {
                            Atom actualType;
                            int actualFormat;
                            unsigned long nItems, bytesAfter;
                            unsigned char *data = nullptr;

                            if (Success == XGetWindowProperty(
                                    dpy, win, ev.xselection.property,
                                    0, (~0L), True, AnyPropertyType,
                                    &actualType, &actualFormat,
                                    &nItems, &bytesAfter, &data)) {

                                if (data && nItems > 0) {
                                    result.assign(reinterpret_cast<char*>(data), nItems);
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
        if (result.empty())
            result = convertAndWait(stringAtom);

        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);

        return result;
    }

    // ------------------------------------------------------------
    // Clear clipboard
    // ------------------------------------------------------------
    void X11::DoClear() {
        enqueue([this] {
            if (!initialized) return;

            {
                std::lock_guard<std::mutex> lock(clipboardMutex);
                clipboardContent.clear();
            }

            /**
             * In X11 mag elke client de owner van een selection zetten, ook als
             * hij hem niet bezit. Onvoorwaardelijk disownen sloopt dus de
             * selectie die de game (Wine) net gepubliceerd heeft na een klik.
             * Alleen loslaten wat van onszelf is.
             */
            if (XGetSelectionOwner(display, clipboardAtom) != window) {
                return;
            }

            XSetSelectionOwner(display, clipboardAtom, None, CurrentTime);
            XFlush(display);

            // Optioneel: ook hier Wine syncen, maar meestal niet nodig
            // ForceWineSync();
        });
    }

}
#endif
