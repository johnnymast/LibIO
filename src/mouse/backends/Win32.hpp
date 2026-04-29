#pragma once

#include "LibIO/MouseControls.hpp"
#include "../../../include/public/LibIO/export.hpp"

#if PLATFORM_WINDOWS
namespace LibIO::Mouse::Backends {
    class LIBIO_API Win32 final : public MouseControls {
    public:
        ~Win32() override = default;

        Win32() = default;

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
    };
}
#endif
