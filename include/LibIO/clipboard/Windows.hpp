#pragma once



#if PLATFORM_WINDOWS
#include "ClipboardControls.hpp"
#include "LibIO/export.hpp"

#include <string>

namespace LibIO::Clipboard {

    class Windows : public ClipboardControls {
    public:
        static ClipboardControls &getInstance();

        void Copy(std::string text) override;
        std::string Paste() override;
        void Clear() override;

        ~Windows() override = default;
    };

}
#endif
