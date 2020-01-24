#include "Transform2D.h"
#include "Actor.h"

Transform2D::Transform2D() :
    mWorldTransform(Matrix4::identity),
    mPosition(Vector3::zero),
    mRotation(Quaternion::identity),
    mPivot(Vector2::zero),
    mScale(Vector2::one),
    mSize(Vector2::zero),
    mIsRecomputeTransform(true) {
    //setPrimary(50);
    float c = static_cast<float>(mCount) / 50000.f;
    mPosition.z = 0.5f - c;
    mPosition.z = Math::clamp<float>(mPosition.z, 0.f, 1.f);
}

Transform2D::~Transform2D() {
    mCount--;
}

bool Transform2D::computeWorldTransform() {
    if (mIsRecomputeTransform) {
        mWorldTransform = Matrix4::createScale(Vector3(mSize, 1.f)); //テクスチャサイズに
        mWorldTransform *= Matrix4::createTranslation(-Vector3(mSize / 2.f + mPivot, 0.f)); //中心 + ピボットを原点に
        mWorldTransform *= Matrix4::createScale(Vector3(getScale(), 1.f));
        mWorldTransform *= Matrix4::createFromQuaternion(getRotation());
        mWorldTransform *= Matrix4::createTranslation(Vector3(getPosition(), mPosition.z));

        mIsRecomputeTransform = false;

        return true;
    }
    return false;
}

Matrix4 Transform2D::getWorldTransform() const {
    return mWorldTransform;
}

void Transform2D::setPosition(const Vector2& pos) {
    mPosition.x = pos.x;
    mPosition.y = pos.y;
    shouldRecomputeTransform();
}

Vector2 Transform2D::getPosition() const {
    return Vector2(mPosition.x, mPosition.y);
}

void Transform2D::translate(const Vector2& translation) {
    mPosition.x += translation.x;
    mPosition.y += translation.y;
    shouldRecomputeTransform();
}

void Transform2D::setPrimary(int primary) { //とりあえず
    float p = static_cast<float>(primary) / 125.f;
    float c = static_cast<float>(mCount) / 50000.f;
    mPosition.z = p - c + 0.1f;
    mPosition.z = Math::clamp<float>(mPosition.z, 0.f, 1.f);
    shouldRecomputeTransform();
    zSortFlag = true;
}

float Transform2D::getDepth() const {
    return mPosition.z;
}

void Transform2D::setRotation(float angle) {
    angle *= 0.5f;
    float sinAngle = Math::sin(angle);

    mRotation.z = sinAngle;
    mRotation.w = Math::cos(angle);

    shouldRecomputeTransform();
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

    shouldRecomputeTransform();
}

void Transform2D::setPivot(const Vector2& pivot) {
    mPivot = pivot;
    shouldRecomputeTransform();
}

Vector2 Transform2D::getPivot() const {
    return mPivot;
}

void Transform2D::setScale(const Vector2& scale) {
    mScale = scale;
    shouldRecomputeTransform();
}

void Transform2D::setScale(float scale) {
    mScale.x = scale;
    mScale.y = scale;
    shouldRecomputeTransform();
}

Vector2 Transform2D::getScale() const {
    return mScale;
}

void Transform2D::setSize(const Vector2& size) {
    mSize = size;
    shouldRecomputeTransform();
}

Vector2 Transform2D::getSize() const {
    return mSize;
}


void Transform2D::shouldRecomputeTransform() {
    mIsRecomputeTransform = true;
}

bool Transform2D::zSortFlag = false;
int Transform2D::mCount = 0;
