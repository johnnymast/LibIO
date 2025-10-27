#pragma once

namespace LibIO::Mouse {
    class MouseControls {
    public:
        virtual ~MouseControls() = default;

        virtual void ClickButton(int button);

        virtual void MoveCursor(int x, int y);

        virtual void LeftClick();

        virtual void RightClick();

        virtual void MoveAndLeftClick(int x, int y);

        virtual void MoveAndRightClick(int x, int y);

        virtual void Scroll(int amount, int* x = nullptr, int* y = nullptr);

        virtual void ScrollUp(int amount = 1);

        virtual void ScrollDown(int amount = 1);

    protected:
        const int Button1 = 1; // Left button
        const int Button2 = 2; // Middle button
        const int Button3 = 3; // Right button
        const int Button4 = 4; // Scroll up
        const int Button5 = 5; // Scroll down
    };
}