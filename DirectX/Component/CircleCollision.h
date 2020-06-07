#pragma once

#include "Collider.h"
#include "../Math/Math.h"
#include "../Utility/Collision.h"
#include <memory>

class SpriteComponent;

class CircleCollision : public Collider {
public:
    CircleCollision();
    ~CircleCollision();
    virtual void start() override;
    virtual void update() override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void set(const Vector2& center, float radius);
    const Circle& getCircle() const;

private:
    void circleUpdate();

private:
    std::shared_ptr<SpriteComponent> mSprite;
    std::unique_ptr<Circle> mCircle;
};
