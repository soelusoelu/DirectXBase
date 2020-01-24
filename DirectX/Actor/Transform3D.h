#pragma once

#include "../Utility/Math.h"
#include <memory>
#include <list>

class Actor;

class Transform3D : public std::enable_shared_from_this<Transform3D> {
public:
    Transform3D(Actor* owner = nullptr);
    ~Transform3D();

    //�A�^�b�`���̃A�N�^�[��Ԃ�
    Actor* getOwner() const;

    //���[���h�s��X�V
    bool computeWorldTransform();
    Matrix4 getWorldTransform() const;

    //�s�N�Z���P�ʂňʒu�w��
    void setPosition(const Vector3& pos);
    Vector3 getPosition() const;
    const Vector3& getLocalPosition() const;
    void translate(const Vector3& translation);

    //��]
    void setRotation(const Quaternion& rot);
    void setRotation(const Vector3& axis, float angle);
    Quaternion getRotation() const;
    const Quaternion& getLocalRotation() const;
    void rotate(const Vector3& axis, float angle);

    //��]�k���s�{�b�g�ʒu
    void setPivot(const Vector3& pivot);
    const Vector3& getPivot() const;

    //�{���Ŋg�k
    void setScale(const Vector3& scale);
    void setScale(float scale);
    Vector3 getScale() const;
    const Vector3& getLocalScale() const;

    //����
    Vector3 forward() const;
    Vector3 right() const;

    //�e�q�֌W
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
