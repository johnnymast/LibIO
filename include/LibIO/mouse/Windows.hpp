#pragma once

#include "LibIO/internals/export.hpp"
#include "MouseControls.hpp"

#if PLATFORM_WINDOWS
namespace LibIO::Mouse {
    class LIBGRAPHICS_API Windows final : public MouseControls {
    public:

        ~Windows() override = default;
        static Windows &getInstance();

        void ClickButton(int button) override;
        void MoveCursor(int x, int y) override;
        void LeftClick() override;
        void RightClick() override;
        void MoveAndLeftClick(int x, int y) override;
        void MoveAndRightClick(int x, int y) override;
        void Scroll(int amount, int* x, int* y) override;;
        void ScrollUp(int amount) override;
        void ScrollDown(int amount) override;
    private:
        Windows() = default;
    };
}
#endif
