#pragma once

#include "Component.h"

class Text;

class Result : public Component {
public:
    Result();
    ~Result();
    virtual void start() override;
    virtual void update() override;
    void setScore(int score);

private:
    std::shared_ptr<Text> mText;
    int mScore;
};
