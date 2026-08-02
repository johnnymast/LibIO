#include "LibIO/ClipboardControls.hpp"

#include <thread>
#include <utility>

namespace LibIO::Clipboard {

    std::recursive_mutex &ClipboardControls::Mutex() {
        static std::recursive_mutex mutex;
        return mutex;
    }

    ClipboardControls::Transaction::Transaction() {
        Mutex().lock();
    }

    ClipboardControls::Transaction::~Transaction() {
        Mutex().unlock();
    }

    void ClipboardControls::Copy(std::string text) {
        std::lock_guard<std::recursive_mutex> lock(Mutex());
        DoCopy(std::move(text));
    }

    std::string ClipboardControls::Paste() {
        std::lock_guard<std::recursive_mutex> lock(Mutex());
        return DoPaste();
    }

    void ClipboardControls::Clear() {
        std::lock_guard<std::recursive_mutex> lock(Mutex());
        DoClear();
    }

    bool ClipboardControls::CopyAndConfirm(const std::string &text, const std::chrono::milliseconds timeout) {
        std::lock_guard<std::recursive_mutex> lock(Mutex());

        DoCopy(text);

        /**
         * Backends mogen asynchroon zijn (X11 zet het werk op een job queue),
         * dus pollen tot de inhoud er echt staat in plaats van aannemen dat
         * DoCopy klaar is als hij terugkeert.
         */
        const auto deadline = std::chrono::steady_clock::now() + timeout;

        while (std::chrono::steady_clock::now() < deadline) {
            if (DoPaste() == text) {
                return true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        return false;
    }

    std::string ClipboardControls::WaitForChange(const std::string &previous,
                                                 const std::chrono::milliseconds timeout) {
        std::lock_guard<std::recursive_mutex> lock(Mutex());

        const auto deadline = std::chrono::steady_clock::now() + timeout;

        while (std::chrono::steady_clock::now() < deadline) {
            if (auto current = DoPaste(); !current.empty() && current != previous) {
                return current;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        return {};
    }
}
