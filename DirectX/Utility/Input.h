#pragma once

#include "../System/DirectXIncLib.h"
#include <dinput.h>
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "dinput8.lib")

enum class KeyCode {
    A = DIK_A,
    B = DIK_B,
    C = DIK_C,
    D = DIK_D,
    E = DIK_E,
    F = DIK_F,
    G = DIK_G,
    H = DIK_H,
    I = DIK_I,
    J = DIK_J,
    K = DIK_K,
    L = DIK_L,
    M = DIK_M,
    N = DIK_N,
    O = DIK_O,
    P = DIK_P,
    Q = DIK_Q,
    R = DIK_R,
    S = DIK_S,
    T = DIK_T,
    U = DIK_U,
    V = DIK_V,
    W = DIK_W,
    X = DIK_X,
    Y = DIK_Y,
    Z = DIK_Z,
    Space = DIK_SPACE,
    LeftShift = DIK_LSHIFT,
    RightShift = DIK_RSHIFT,
    LeftControl = DIK_LCONTROL,
    RightControl = DIK_RCONTROL,
    LeftAlt = DIK_LALT,
    RightAlt = DIK_RALT,
    Tab = DIK_TAB,
    Escape = DIK_ESCAPE,
    UpArrow = DIK_UPARROW,
    DownArrow = DIK_DOWNARROW,
    LeftArrow = DIK_LEFTARROW,
    RightArrow = DIK_RIGHTARROW,
    Alpha1 = DIK_1,
    Alpha2 = DIK_2,
    Alpha3 = DIK_3,
    Alpha4 = DIK_4,
    Alpha5 = DIK_5,
    Alpha6 = DIK_6,
    Alpha7 = DIK_7,
    Alpha8 = DIK_8,
    Alpha9 = DIK_9,
    Alpha0 = DIK_0,
    F1 = DIK_F1,
    F2 = DIK_F2,
    F3 = DIK_F3,
    F4 = DIK_F4,
    F5 = DIK_F5,
    F6 = DIK_F6,
    F7 = DIK_F7,
    F8 = DIK_F8,
    F9 = DIK_F9,
    F10 = DIK_F10,
    F11 = DIK_F11,
    F12 = DIK_F12,

    None = 512
};
enum class JoyCode {
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    LeftButton = 4,
    RightButton = 5,
    ViewButton = 6,
    MenuButton = 7,
    LeftStickButton = 8,
    RightStickButton = 9,
};

class Input {
public:
    static HRESULT init(HWND hWnd);
    static void end();
    static void update();
    //キーが押された瞬間
    static bool getKeyDown(KeyCode key);
    static bool getJoyStickDown(JoyCode joy);
    //キーが押され続けているか
    static bool getKey(KeyCode key);
    static bool getJoy(JoyCode joy);
    //キーが離れた瞬間
    static bool getKeyUp(KeyCode key);
    static bool getJoyUp(JoyCode joy);
    //縦横
    static int horizontal();
    static int vertical();

public:
    static LPDIRECTINPUT8 mDinput;
    static LPDIRECTINPUTDEVICE8 mPadDevice;

private:
    static BYTE mCurrentKeys[256];
    static BYTE mPreviousKeys[256];
    static DIJOYSTATE2 mCurrentJoyState;
    static DIJOYSTATE2 mPreviousJoyState;

    static LPDIRECTINPUTDEVICE8 mKeyDevice;
};

