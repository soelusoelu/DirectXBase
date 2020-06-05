#pragma once

#include "IMouse.h"
#include <dinput.h>
#include <string>

class Mouse : public IMouse {
public:
    Mouse();
    ~Mouse();
    virtual bool getMouseButtonDown(MouseCode button) override;
    virtual bool getMouseButton(MouseCode button) override;
    virtual bool getMouseButtonUp(MouseCode button) override;
    virtual Vector2 getMousePosition() override;

    bool initialize(HWND hWnd, IDirectInput8* directInput);
    void update();
    //文字列をMouseCodeに変換
    static void stringToJoyCode(const std::string& src, MouseCode* dst);

private:
    IDirectInputDevice8* mMouseDevice;
    DIMOUSESTATE2 mCurrentMouseState;
    DIMOUSESTATE2 mPreviousMouseState;
    HWND mhWnd;
    float mMousePositionX;
    float mMousePositionY;
};
