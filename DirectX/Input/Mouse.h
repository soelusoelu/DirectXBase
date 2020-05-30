#pragma once

#include "../Math/Math.h"
#include <dinput.h>
#include <string>

enum class MouseCode {
    LeftButton = 0,
    RightButton = 1,
    CenterButton = 2,
    SideButton1 = 3,
    SideButton2 = 4,
    SideButton3 = 5,

    None = 512
};

class Mouse {
public:
    Mouse();
    ~Mouse();
    bool initialize(HWND hWnd, IDirectInput8* directInput);
    void update();
    //キーが押された瞬間
    bool getMouseDown(MouseCode button);
    //キーが押され続けているか
    bool getMouse(MouseCode button);
    //キーが離れた瞬間
    bool getMouseUp(MouseCode button);
    //マウスのウィンドウ位置
    const Vector2& getMousePosition();
    //文字列をMouseCodeに変換
    static void stringToJoyCode(const std::string& src, MouseCode* dst);

private:
    IDirectInputDevice8* mMouseDevice;
    DIMOUSESTATE2 mCurrentMouseState;
    DIMOUSESTATE2 mPreviousMouseState;
    HWND mhWnd;
    Vector2 mMousePosition;
};
