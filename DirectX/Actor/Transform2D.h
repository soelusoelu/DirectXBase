#pragma once

#include "../Utility/Math.h"

class Transform2D {
public:
    Transform2D();
    ~Transform2D();

    //ワールド行列更新
    bool computeWorldTransform(const Vector2INT& texSize, const Vector2& pivot, float z);
    Matrix4 getWorldTransform() const;

    //ピクセル単位で位置指定
    void setPosition(const Vector2& pos);
    Vector2 getPosition() const;
    void translate(const Vector2& translation);

    //回転
    void setRotation(float angle);
    Quaternion getRotation() const;
    void rotate(float angle);

    //倍率で拡縮
    void setScale(const Vector2& scale);
    void setScale(float scale);
    Vector2 getScale() const;

private:
    Matrix4 mWorldTransform;
    Vector2 mPosition;
    Quaternion mRotation;
    Vector2 mScale;
    bool mIsRecomputeTransform;
};

