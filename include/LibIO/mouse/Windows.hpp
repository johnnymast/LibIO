#pragma once

#if PLATFORM_WINDOWS
#include "MouseControls.hpp"

namespace LibIO::Mouse {
    class Windows final : public MouseControls {
    public:

        ~Windows() override = default;
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
        Windows() = default;
    };
}
#endif
