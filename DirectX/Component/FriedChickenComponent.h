#pragma once

#include "Component.h"
#include "../Utility/Math.h"
#include <rapidjson/document.h>
#include <memory>

class Actor;

class FriedChickenComponent : public Component {
public:
    FriedChickenComponent(std::shared_ptr<Actor> owner);
    ~FriedChickenComponent();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(debugInfoList* inspect) const override;
    //揚げ直す前の状態に戻す
    void initialize();

private:
    Vector2 mRandomRangePositionX;
    Vector2 mRandomRangePositionZ;
    Vector2 mRandomRangeScale;
};
