#pragma once

#include "LibIO/MouseControls.hpp"

#if PLATFORM_X11
#include <X11/Xlib.h>

namespace LibIO::Mouse::Backends {
    class X11 final : public MouseControls {
    public:
        X11() = default;

        ~X11() override = default;

        static MouseControls &getInstance();

        void ClickButton(int button) override;

        void MoveCursor(int x, int y) override;

        void LeftClick() override;

        void RightClick() override;

        void MoveAndLeftClick(int x, int y) override;

        void MoveAndRightClick(int x, int y) override;

        void Scroll(int amount, int *x, int *y) override;

        void ScrollUp(int amount) override;

        void ScrollDown(int amount) override;

    private:
        static Display *GetDisplay();
    };
}
#endif //LIBIO_X11_HPP
