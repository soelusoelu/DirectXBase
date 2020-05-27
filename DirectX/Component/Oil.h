#pragma once

#include "Component.h"

class GameObject;

class Oil : public Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;

public:
    Oil();
    ~Oil();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void flow(const GameObjectPtr& gameObject);

private:
    float mFlowSpeed;
    float mFlowRangeX;
    float mFlowRangeZFore;
    float mFlowRangeZBack;
};
