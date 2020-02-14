#pragma once

#include "SceneBase.h"
#include "../Utility/Input.h"
#include <memory>

class Title : public SceneBase {
public:
    Title();
    ~Title();
    virtual void start() override;
    virtual void update() override;

private:
    KeyCode mNextKey;
    JoyCode mNextJoy;
};
