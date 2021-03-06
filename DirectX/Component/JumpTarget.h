﻿#pragma once

#include "../Component/Component.h"
#include "../Math/Math.h"
#include <memory>

class Sprite3D;

class JumpTarget : public Component {
public:
    JumpTarget();
    ~JumpTarget();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    void setActive(bool value);
    void setTargetPosition(const Vector3& pos);

private:
    std::shared_ptr<Sprite3D> mSprite;
    float mOffsetPosY;
    float mAmountRotation;
};
