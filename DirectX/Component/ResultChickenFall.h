#pragma once

#include "Component.h"
#include <memory>

class SpriteComponent;

class ResultChickenFall : public Component {
public:
    ResultChickenFall();
    ~ResultChickenFall();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void initialize();

private:
    std::shared_ptr<SpriteComponent> mSprite;
    float mRandomFallSpeedMin;
    float mRandomFallSpeedMax;
    float mFallSpeed;
    float mRotateSpeed;
};
