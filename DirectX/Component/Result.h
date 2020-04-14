#pragma once

#include "Component.h"

class Result : public Component {
public:
    Result(std::shared_ptr<GameObject> owner);
    ~Result();
    virtual void update() override;
    void setScore(int score);

private:
    int mScore;
};
