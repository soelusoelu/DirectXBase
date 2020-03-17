#pragma once

#include "../Utility/Math.h"
#include <rapidjson/document.h>
#include <memory>

class Actor;
class Component;
class DrawString;
class Renderer;
class Transform3D;

class Inspector {
    friend class InspectHelper;

    using ActorPtr = std::shared_ptr<Actor>;
    using ComponentPtr = std::shared_ptr<Component>;
    using TransformPtr = std::shared_ptr<Transform3D>;

public:
    Inspector(DrawString* drawString);
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
    DrawString* mDrawString;
    std::weak_ptr<Actor> mTarget;
    float mInspectorPositionX;
    Vector2 mTagScale;
    Vector2 mElementScale;
    Vector2 mTransformPosition;
    Vector2 mComponentPosition;
    //変数名のX軸の位置
    float mElementPositionX;
    //値のX軸の位置
    float mValuePositionX;
    //表示位置を右にずらす文字数
    int mOffsetCharCountX;
    //表示位置を右にずらす分量
    float mOffsetX;
    //1文字の大きさ
    float mCharWidth;
    float mCharHeight;
};

class InspectHelper {
public:
    static std::string vector2ToString(const Vector2& vec);
    static std::string vector3ToString(const Vector3& vec);
    static std::string quaternionToString(const Quaternion& quaternion);
};
