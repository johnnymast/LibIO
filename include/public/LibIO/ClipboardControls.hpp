#pragma once

#include "LibIO/export.hpp"
#include <string>

namespace LibIO::Clipboard {
    class LIBIO_API ClipboardControls {
    public:
        virtual ~ClipboardControls() = default;

        virtual void Copy(std::string text) = 0;
        virtual std::string Paste() = 0;
        virtual void Clear() = 0;
    };
}