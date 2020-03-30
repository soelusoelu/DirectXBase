#pragma once

#include "../Component/Component.h"
#include "../Math/Math.h"
#include <memory>

class SpriteComponent;

class JumpTarget : public Component {
public:
    JumpTarget(std::shared_ptr<GameObject> owner);
    ~JumpTarget();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    void setTargetPosition(const Vector2& pos);

private:
    std::shared_ptr<SpriteComponent> mSpriteComp;
    Vector2 mPosition;
    Vector2 mTextureScale;
};
