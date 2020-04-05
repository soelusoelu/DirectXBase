#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>

class Component;
class DrawString;
class GameObject;
class Renderer;
class Transform3D;

class Inspector {
    friend class InspectHelper;

    using GameObjectPtr = std::shared_ptr<GameObject>;
    using ComponentPtr = std::shared_ptr<Component>;
    using TransformPtr = std::shared_ptr<Transform3D>;

public:
    Inspector(DrawString* drawString);
    ~Inspector();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void setTarget(const GameObjectPtr& target);
    void drawInspect() const;

private:
    void drawTag(const GameObjectPtr& target) const;
    void drawTransform(const TransformPtr& target) const;
    void drawPosition(const TransformPtr& target, const Vector2& position) const;
    void drawRotation(const TransformPtr& target, const Vector2& position) const;
    void drawScale(const TransformPtr& target, const Vector2& position) const;
    void drawComponent(const ComponentPtr& component, Vector2* position) const;

private:
    DrawString* mDrawString;
    std::weak_ptr<GameObject> mTarget;
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
    //変数名の表示限界文字数
    int mMaxElementCharCount;
};

class InspectHelper {
public:
    static std::string vector2ToString(const Vector2& vec);
    static std::string vector3ToString(const Vector3& vec);
    static std::string quaternionToString(const Quaternion& quaternion);

private:
    static std::string minusPosition(float value);
};
