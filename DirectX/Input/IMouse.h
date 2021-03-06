﻿#pragma once

class Vector2;

enum class MouseCode {
    LeftButton = 0,
    RightButton = 1,
    CenterButton = 2,
    SideButton1 = 3,
    SideButton2 = 4,
    SideButton3 = 5,

    None = 512
};

class IMouse {
public:
    virtual ~IMouse() = default;
    //キーが押された瞬間
    virtual bool getMouseButtonDown(MouseCode button) = 0;
    //キーが押され続けているか
    virtual bool getMouseButton(MouseCode button) = 0;
    //キーが離れた瞬間
    virtual bool getMouseButtonUp(MouseCode button) = 0;
    //マウスのウィンドウ位置
    virtual Vector2 getMousePosition() = 0;
};
