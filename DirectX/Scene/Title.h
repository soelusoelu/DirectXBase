#pragma once

#include "SceneBase.h"
#include "../Utility/Input.h"
#include <memory>

class Title : public SceneBase {
public:
    Title();
    ~Title();
    virtual void startScene() override;
    virtual void updateScene() override;

private:
    KeyCode mNextKey;
};

