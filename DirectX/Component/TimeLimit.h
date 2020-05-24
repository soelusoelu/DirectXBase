#pragma once

#include "Component.h"
#include <memory>

class Text;
class Timer;

class TimeLimit : public Component {
public:
    TimeLimit();
    ~TimeLimit();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<Text> mText;
    std::shared_ptr<Timer> mTimer;
};
