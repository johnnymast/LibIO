#pragma once

#if PLATFORM_WINDOWS
#include "LibIO/ClipboardControls.hpp"
#include "LibIO/export.hpp"

#include <string>

namespace LibIO::Clipboard::Backends {

    class LIBIO_API Win32 : public ClipboardControls {
    public:

        ~Win32() override = default;

        static ClipboardControls &getInstance();

        void Copy(std::string text) override;
        std::string Paste() override;
        void Clear() override;


    };

}
#endif
