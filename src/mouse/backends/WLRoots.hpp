#pragma once

#include "LibIO/MouseControls.hpp"

#if PLATFORM_WLROOTS
namespace LibIO::Mouse::Backends {
    class WLRoots : public MouseControls {
        public:
        ~WLRoots() override = default;
        static MouseControls &getInstance();

        void ClickButton(int button) override;
        void MoveCursor(int x, int y) override;
        void LeftClick() override;
        void RightClick() override;
        void MoveAndLeftClick(int x, int y) override;
        void MoveAndRightClick(int x, int y) override;
        void Scroll(int amount, int* x, int* y) override;
        void ScrollUp(int amount) override;
    };
}
#endif
