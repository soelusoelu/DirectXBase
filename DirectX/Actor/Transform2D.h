#pragma once

#include "../Utility/Math.h"

class Transform2D {
public:
    Transform2D();
    ~Transform2D();

    //ワールド行列更新
    bool computeWorldTransform();
    Matrix4 getWorldTransform() const;

    //ピクセル単位で位置指定
    void setPosition(const Vector2& pos);
    Vector2 getPosition() const;
    void translate(const Vector2& translation);

    //描画優先順位
    void setPrimary(float z);
    float getDepth() const;

    //回転
    void setRotation(float angle);
    Quaternion getRotation() const;
    void rotate(float angle);

    //回転ピボット位置
    void setPivot(const Vector2& pivot);
    Vector2 getPivot() const;

    //倍率で拡縮
    void setScale(const Vector2& scale);
    void setScale(float scale);
    Vector2 getScale() const;

private:
    Matrix4 mWorldTransform;
    Vector3 mPosition;
    Quaternion mRotation;
    Vector2 mPivot;
    Vector2 mScale;
    bool mIsRecomputeTransform;
};

