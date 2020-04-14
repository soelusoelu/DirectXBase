#pragma once

#include "SceneBase.h"

class ResultScene : public SceneBase {
public:
    ResultScene(int score);
    ~ResultScene();
    virtual void start() override;
    virtual void update() override;

private:
    int mScore;
};
