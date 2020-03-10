#pragma once

#include "Math.h"
#include <dinput.h>

enum class MouseCode {
    LeftButton = 0,
    RightButton = 1,
    CenterButton = 2,
    SideButton1 = 3,
    SideButton2 = 4,
    SideButton3 = 5
};

class Mouse {
public:
    Mouse();
    ~Mouse();
    bool initialize(HWND hWnd, LPDIRECTINPUT8 directInput);
    void update();
    //キーが押された瞬間
    bool getMouseDown(MouseCode button);
    //キーが押され続けているか
    bool getMouse(MouseCode button);
    //キーが離れた瞬間
    bool getMouseUp(MouseCode button);
    //マウスのウィンドウ位置
    const Vector2& getMousePosition();

private:
    DIMOUSESTATE2 mCurrentMouseState;
    DIMOUSESTATE2 mPreviousMouseState;
    LPDIRECTINPUTDEVICE8 mMouseDevice;
    Vector2 mMousePosition;
    HWND mhWnd;
};
