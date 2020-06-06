#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <memory>

class Sprite3D;

class ChickenExclamation : public Component {
public:
    ChickenExclamation();
    ~ChickenExclamation();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void setActive(bool value);
    void setPosition(const Vector3& pos);
    float getTime() const;

private:
    std::shared_ptr<Sprite3D> mSprite;
    float mBurntSecondsAgo;
    Vector3 mOffsetPosition;
    float mMeshRadius;
};

