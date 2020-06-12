#pragma once

#include "Component.h"
#include <memory>

class TextFloat;
class Time;

class TimeLimit : public Component {
public:
    TimeLimit();
    ~TimeLimit();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    bool isTime() const;

private:
    std::shared_ptr<TextFloat> mText;
    std::unique_ptr<Time> mTimer;
};
