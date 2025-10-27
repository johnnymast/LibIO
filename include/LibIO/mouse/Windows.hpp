#pragma once

#include "MouseControls.hpp"

namespace LibIO::Mouse {
    class Windows final : public MouseControls {
    public:

        void ClickButton(int button) override;

        void MoveCursor(int x, int y) override;

        void LeftClick() override;

        void RightClick() override;

        void MoveAndLeftClick(int x, int y) override;

        void MoveAndRightClick(int x, int y) override;

        void Scroll(int amount, int* x = nullptr, int* y = nullptr) override;;

        void ScrollUp(int amount) override;

        void ScrollDown(int amount) override;
    };
}

