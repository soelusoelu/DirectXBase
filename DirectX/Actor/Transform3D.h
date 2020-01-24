#pragma once

#include "../Utility/Math.h"
#include <memory>
#include <list>

class Actor;

class Transform3D : public std::enable_shared_from_this<Transform3D> {
public:
    Transform3D(Actor* owner = nullptr);
    ~Transform3D();

    //アタッチ元のアクターを返す
    Actor* getOwner() const;

    //ワールド行列更新
    bool computeWorldTransform();
    Matrix4 getWorldTransform() const;

    //ピクセル単位で位置指定
    void setPosition(const Vector3& pos);
    Vector3 getPosition() const;
    const Vector3& getLocalPosition() const;
    void translate(const Vector3& translation);

    //回転
    void setRotation(const Quaternion& rot);
    void setRotation(const Vector3& axis, float angle);
    Quaternion getRotation() const;
    const Quaternion& getLocalRotation() const;
    void rotate(const Vector3& axis, float angle);

    //回転縮小ピボット位置
    void setPivot(const Vector3& pivot);
    const Vector3& getPivot() const;

    //倍率で拡縮
    void setScale(const Vector3& scale);
    void setScale(float scale);
    Vector3 getScale() const;
    const Vector3& getLocalScale() const;

    //向き
    Vector3 forward() const;
    Vector3 right() const;

    //親子関係
    void addChild(std::shared_ptr<Transform3D> child);
    void removeChild(std::shared_ptr<Transform3D> child);
    void removeChild(const char* tag);
    std::shared_ptr<Transform3D> getChild(const char* tag) const;
    std::shared_ptr<Transform3D> parent() const;
    std::shared_ptr<Transform3D> root() const;
    size_t getChildCount() const;

private:
    void setParent(std::shared_ptr<Transform3D> parent);
    void shouldRecomputeTransform();

private:
    Actor* mOwner;
    Matrix4 mWorldTransform;
    Vector3 mPosition;
    Quaternion mRotation;
    Vector3 mPivot;
    Vector3 mScale;
    std::weak_ptr<Transform3D> mParent;
    std::list<std::shared_ptr<Transform3D>> mChildren;
    bool mIsRecomputeTransform;
};
