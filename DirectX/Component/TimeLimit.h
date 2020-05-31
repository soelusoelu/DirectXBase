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
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    std::shared_ptr<Text> mText;
    std::shared_ptr<Timer> mTimer;
    int mDecimalDigit;
};
