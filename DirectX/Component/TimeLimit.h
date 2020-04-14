#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <memory>

class Time;

class TimeLimit : public Component {
public:
    TimeLimit(std::shared_ptr<GameObject> owner);
    ~TimeLimit();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    bool isOverLimit() const;

private:
    std::unique_ptr<Time> mTimer;
    float mLimit;
    Vector2 mDrawPosition;
    Vector2 mDrawScale;
};
