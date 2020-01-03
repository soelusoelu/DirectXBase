#include "Transform2D.h"

Transform2D::Transform2D() :
    mWorldTransform(Matrix4::identity),
    mPosition(Vector3::zero),
    mRotation(Quaternion::identity),
    mPivot(Vector2::zero),
    mScale(Vector2::one),
    mIsRecomputeTransform(true) {
}

Transform2D::~Transform2D() = default;

bool Transform2D::computeWorldTransform() {
    if (mIsRecomputeTransform) {
        mIsRecomputeTransform = false;

        mWorldTransform = Matrix4::createTranslation(-Vector3(mPivot, 0.f));
        mWorldTransform *= Matrix4::createScale(Vector3(mScale, 1.f));
        mWorldTransform *= Matrix4::createFromQuaternion(mRotation);
        mWorldTransform *= Matrix4::createTranslation(mPosition + Vector3(mPivot, 0.f));

        return true;
    }
    return false;
}

Matrix4 Transform2D::getWorldTransform() const {
    return mWorldTransform;
}

void Transform2D::setPosition(const Vector2 & pos) {
    mPosition.x = pos.x;
    mPosition.y = pos.y;
    mIsRecomputeTransform = true;
}

Vector2 Transform2D::getPosition() const {
    return Vector2(mPosition.x, mPosition.y);
}

void Transform2D::translate(const Vector2 & translation) {
    mPosition.x += translation.x;
    mPosition.y += translation.y;
    mIsRecomputeTransform = true;
}

void Transform2D::setPrimary(float z) {
    mPosition.z = z;
    mIsRecomputeTransform = true;
}

float Transform2D::getDepth() const {
    return mPosition.z;
}

void Transform2D::setRotation(float angle) {
    angle *= 0.5f;
    float sinAngle = Math::sin(angle);

    mRotation.z = sinAngle;
    mRotation.w = Math::cos(angle);

    mIsRecomputeTransform = true;
}

Quaternion Transform2D::getRotation() const {
    return mRotation;
}

void Transform2D::rotate(float angle) {
    angle *= 0.5f;
    float sinAngle = Math::sin(angle);

    Quaternion inc;
    inc.z = sinAngle;
    inc.w = Math::cos(angle);

    mRotation = Quaternion::concatenate(mRotation, inc);

    mIsRecomputeTransform = true;
}

void Transform2D::setPivot(const Vector2 & pivot) {
    mPivot = pivot;
    mIsRecomputeTransform = true;
}

Vector2 Transform2D::getPivot() const {
    return mPivot;
}

void Transform2D::setScale(const Vector2 & scale) {
    mScale = scale;
    mIsRecomputeTransform = true;
}

void Transform2D::setScale(float scale) {
    mScale.x = scale;
    mScale.y = scale;
    mIsRecomputeTransform = true;
}

Vector2 Transform2D::getScale() const {
    return mScale;
}
