#pragma once

#include "LibIO/internals/export.hpp"

#include <string>

namespace LibIO::Clipboard {
    class LIBIO_API ClipboardControls {
    public:

        virtual ~ClipboardControls() = default;

        virtual void Copy(std::string text);
        virtual std::string Paste();
        virtual void Clear();
    };
}