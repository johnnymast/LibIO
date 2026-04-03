#pragma once

#include <string>

#include "LibIO/internals/export.hpp"

namespace LibIO::Clipboard {
    class LIBGRAPHICS_API ClipboardControls {
    public:

        virtual ~ClipboardControls() = default;

        virtual void Copy(std::string text);
        virtual std::string Paste();
        virtual void Clear();
    };
}