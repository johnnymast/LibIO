#pragma once

#include "LibIO/internals/export.hpp"
#include "MouseControls.hpp"

#if defined(PLATFORM_LINUX)
#include <X11/Xlib.h>

namespace LibIO::Mouse {
    class LIBGRAPHICS_API Linux final : public MouseControls {
    public:

        ~Linux() override = default;
        static MouseControls &getInstance();

        void ClickButton(int button) override;
        void MoveCursor(int x, int y) override;
        void LeftClick() override;
        void RightClick() override;
        void MoveAndLeftClick(int x, int y) override;
        void MoveAndRightClick(int x, int y) override;
        void Scroll(int amount, int* x, int* y) override;
        void ScrollUp(int amount) override;
        void ScrollDown(int amount) override;
    private:
        Linux() = default;
        static Display* GetDisplay();
    };
}
#endif
