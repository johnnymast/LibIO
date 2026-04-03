#pragma once

#include "LibIO/internals/export.hpp"

namespace LibIO::Mouse {
    class LIBGRAPHICS_API MouseControls {
    public:

        virtual ~MouseControls() = default;
        virtual void ClickButton(int button);
        virtual void MoveCursor(int x, int y);
        virtual void LeftClick();
        virtual void RightClick();
        virtual void MoveAndLeftClick(int x, int y);
        virtual void MoveAndRightClick(int x, int y);
        virtual void Scroll(int amount, int* x, int* y);
        virtual void ScrollUp(int amount);
        virtual void ScrollDown(int amount);
    };
}