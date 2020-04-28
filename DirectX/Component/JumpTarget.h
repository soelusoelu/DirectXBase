#pragma once

#include "../Component/Component.h"
#include "../Math/Math.h"

class JumpTarget : public Component {
public:
    JumpTarget(std::shared_ptr<GameObject> owner);
    ~JumpTarget();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    void setTargetPosition(const Vector3& pos);

private:
    float mOffsetPosY;
};
