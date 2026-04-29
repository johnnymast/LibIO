#pragma once

#include "LibIO/ClipboardControls.hpp"

#if PLATFORM_WLROOTS
namespace LibIO::Clipboard::Backends {
    class WLRoots: public ClipboardControls {
    public:

        ~WLRoots() override = default;
        static ClipboardControls &getInstance();

        void Copy(std::string text) override;
        std::string Paste() override;
        void Clear() override;
    };
}
#endif