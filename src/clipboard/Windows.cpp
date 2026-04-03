#include "LibIO/clipboard/Windows.hpp"
#include <stdexcept>

#if PLATFORM_WINDOWS
namespace LibIO::Clipboard {

    ClipboardControls &Windows::getInstance() {
        static Windows instance;
        return instance;
    }

    void Windows::Copy(std::string text) {
        if (!OpenClipboard(nullptr)) return;
        EmptyClipboard();

        // Allocate global memory for the text
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (!hMem) {
            CloseClipboard();
            return;
        }

        char* buffer = static_cast<char*>(GlobalLock(hMem));
        if (buffer) {
            memcpy(buffer, text.c_str(), text.size() + 1);
        }
        GlobalUnlock(hMem);

        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
    }

    std::string Windows::Paste() {
        if (!OpenClipboard(nullptr)) return "";

        HANDLE hData = GetClipboardData(CF_TEXT);
        if (!hData) {
            CloseClipboard();
            return "";
        }

        char* buffer = static_cast<char*>(GlobalLock(hData));
        std::string result;
        if (buffer) {
            result = buffer;
        }
        GlobalUnlock(hData);
        CloseClipboard();

        return result;
    }

    void Windows::Clear() {
        if (OpenClipboard(nullptr)) {
            EmptyClipboard();
            CloseClipboard();
        }
    }

} // namespace LibIO::Clipboard
#endif
