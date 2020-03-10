#pragma once

#include "Math.h"
#include "../System/DirectXIncLib.h"
#include <dinput.h>
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "dinput8.lib")

enum class JoyCode {
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    LeftButton = 4,
    RightButton = 5,
    SelectButton = 6,
    StartButton = 7,
    LeftStickButton = 8,
    RightStickButton = 9,
};

class Keyboard;
class Mouse;

class Input {
public:
    static HRESULT init(HWND hWnd);
    static void end();
    static void update();
    //キーが押された瞬間
    static bool getJoyDown(JoyCode joy);
    //キーが押され続けているか
    static bool getJoy(JoyCode joy);
    //キーが離れた瞬間
    static bool getJoyUp(JoyCode joy);
    //ジョイスティック対応
    static float joyHorizontal();
    static float joyVertical();
    //ジョイスティック右
    static float joyRHorizontal();
    static float joyRVertical();

    static Keyboard* keyboard();
    static Mouse* mouse();

public:
    static LPDIRECTINPUT8 mDinput;
    static LPDIRECTINPUTDEVICE8 mPadDevice;

private:
    static DIJOYSTATE2 mCurrentJoyState;
    static DIJOYSTATE2 mPreviousJoyState;

    static Keyboard* mKeyboard;
    static Mouse* mMouse;
};
