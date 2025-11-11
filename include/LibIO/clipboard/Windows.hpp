#pragma once

#include "ClipboardControls.hpp"

#if PLATFORM_WINDOWS
#include <string>
#include <windows.h>

namespace LibIO::Clipboard {

    class Windows : public ClipboardControls {
    public:
        static ClipboardControls &getInstance();

        void Copy(std::string text) override;
        std::string Paste() override;
        void Clear() override;

        ~Windows() = default;
    };

} // namespace LibIO::Clipboard
#endif
