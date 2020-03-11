#pragma once

#include "Math.h"
#include <dinput.h>

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

class JoyPad {
public:
    JoyPad();
    ~JoyPad();
    bool initialize(HWND hWnd, LPDIRECTINPUT8 directInput);
    void update();
    //キーが押された瞬間
    bool getJoyDown(JoyCode joy) const;
    //キーが押され続けているか
    bool getJoy(JoyCode joy) const;
    //キーが離れた瞬間
    bool getJoyUp(JoyCode joy) const;
    //ジョイスティック対応
    Vector2 leftStick() const;
    Vector2 rightStick() const;

public:
    static LPDIRECTINPUTDEVICE8 mPadDevice;
    static constexpr float STICK_VALUE = 1000.f;

private:
    DIJOYSTATE2 mCurrentJoyState;
    DIJOYSTATE2 mPreviousJoyState;
    static constexpr float DEAD_ZONE = 100.f;
};
