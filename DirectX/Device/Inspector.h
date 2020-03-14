#pragma once

#include "../Utility/Math.h"
#include <rapidjson/document.h>
#include <memory>

class Actor;
class Component;
class DrawString;
class Transform3D;

class Inspector {
    using ActorPtr = std::shared_ptr<Actor>;
    using ComponentPtr = std::shared_ptr<Component>;
    using TransformPtr = std::shared_ptr<Transform3D>;

public:
    Inspector(const std::shared_ptr<DrawString> drawString);
    ~Inspector();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void setTarget(const ActorPtr target);
    void drawInspect() const;

private:
    void drawActorTag(const ActorPtr target) const;
    void drawActorTransform(const TransformPtr target) const;
    void drawPosition(const TransformPtr target, const Vector2& position) const;
    void drawRotation(const TransformPtr target, const Vector2& position) const;
    void drawScale(const TransformPtr target, const Vector2& position) const;
    void drawComponent(const ComponentPtr component, Vector2* position) const;

private:
    std::shared_ptr<DrawString> mDrawString;
    std::weak_ptr<Actor> mTarget;
    Vector2 mTagScale;
    Vector2 mTransformScale;
    Vector2 mTransformPosition;
    float mTransformElementPositionX;
    Vector2 mComponentPosition;
    static const Vector2 TAG_POSITION;
};
