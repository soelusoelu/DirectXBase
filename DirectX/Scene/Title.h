#pragma once

#include "SceneBase.h"

class Title : public SceneBase {
public:
    Title();
    ~Title();
    virtual void start() override;
    virtual void update() override;
};
