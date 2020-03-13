#pragma once

#include "../Utility/Math.h"
#include <memory>

class Actor;
class DrawString;
class Transform3D;

class Inspector {
    using ActorPtr = std::shared_ptr<Actor>;
    using TransformPtr = std::shared_ptr<Transform3D>;

public:
    Inspector(const std::shared_ptr<DrawString> drawString);
    ~Inspector();
    void setTarget(const ActorPtr target);
    void drawInspect() const;

private:
    void drawActorTag(const ActorPtr target) const;
    void drawActorTransform(const TransformPtr target) const;
    void drawPosition(const TransformPtr target, const Vector2& pos) const;
    void drawRotation(const TransformPtr target, const Vector2& pos) const;
    void drawScale(const TransformPtr target, const Vector2& pos) const;

private:
    std::shared_ptr<DrawString> mDrawString;
    std::weak_ptr<Actor> mTarget;
    static const Vector2 TAG_POSITION;
    static const Vector2 TAG_SCALE;
    static const Vector2 TRANSFORM_POSITION;
    static const Vector2 TRANSFORM_SCALE;
    static const float TRANSFORM_ELEMENT_POSITION_X;
};
