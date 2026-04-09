#pragma once

#include "LibIO/export.hpp"

namespace LibIO::Mouse {
    class LIBIO_API MouseControls {
    public:

        virtual ~MouseControls() = default;
        virtual void ClickButton(int button) = 0;
        virtual void MoveCursor(int x, int y) = 0;
        virtual void LeftClick() = 0;
        virtual void RightClick() = 0;
        virtual void MoveAndLeftClick(int x, int y) = 0;
        virtual void MoveAndRightClick(int x, int y) = 0;
        virtual void Scroll(int amount, int* x, int* y) = 0;
        virtual void ScrollUp(int amount) = 0;
        virtual void ScrollDown(int amount) = 0;
    };
}