#pragma once

#include "Component.h"

class GameObject;

class Oil : public Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;

public:
    Oil(std::shared_ptr<GameObject> owner);
    ~Oil();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    void flow(const GameObjectPtr& gameObject);

private:
    float mFlowSpeed;
    float mFlowRangeX;
    float mFlowRangeZFore;
    float mFlowRangeZBack;
};
