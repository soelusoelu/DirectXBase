#pragma once

#include "../Math/Math.h"
#include <dinput.h>
#include <string>

enum class JoyCode {
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    LeftButton = 4,
    RightButton = 5,
    Select = 6,
    Start = 7,
    LeftStickButton = 8,
    RightStickButton = 9,

    None = 512
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
    //文字列をJoyCodeに変換
    static void stringToJoyCode(const std::string& src, JoyCode* dst);

public:
    static LPDIRECTINPUTDEVICE8 mPadDevice;
    static constexpr float STICK_VALUE = 1000.f;

private:
    DIJOYSTATE2 mCurrentJoyState;
    DIJOYSTATE2 mPreviousJoyState;
    static constexpr float DEAD_ZONE = 100.f;
};
