#include "Win32.hpp"

#if PLATFORM_WINDOWS
#include <stdexcept>
#include <windows.h>

namespace LibIO::Clipboard::Backends {
    ClipboardControls &Win32::getInstance() {
        static Win32 instance;
        return instance;
    }

    void Win32::DoCopy(std::string text) {
        if (!OpenClipboard(nullptr)) return;
        EmptyClipboard();

        // Allocate global memory for the text
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (!hMem) {
            CloseClipboard();
            return;
        }

        char *buffer = static_cast<char *>(GlobalLock(hMem));
        if (buffer) {
            memcpy(buffer, text.c_str(), text.size() + 1);
        }
        GlobalUnlock(hMem);

        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
    }

    std::string Win32::DoPaste() {
        if (!OpenClipboard(nullptr)) return "";

        HANDLE hData = GetClipboardData(CF_TEXT);
        if (!hData) {
            CloseClipboard();
            return "";
        }

        char *buffer = static_cast<char *>(GlobalLock(hData));
        std::string result;
        if (buffer) {
            result = buffer;
        }
        GlobalUnlock(hData);
        CloseClipboard();

        return result;
    }

    void Win32::DoClear() {
        if (OpenClipboard(nullptr)) {
            EmptyClipboard();
            CloseClipboard();
        }
    }
} // namespace LibIO::Clipboard
#endif
