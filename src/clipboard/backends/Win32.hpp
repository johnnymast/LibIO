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

    protected:
        void DoCopy(std::string text) override;

        std::string DoPaste() override;

        void DoClear() override;
    };
}
#endif
