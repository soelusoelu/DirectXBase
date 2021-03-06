﻿#pragma once

#include "Pivot.h"
#include "../Math/Math.h"
#include <memory>
#include <list>

class Transform2D {
public:
    Transform2D();
    ~Transform2D();

    //ワールド行列更新
    bool computeWorldTransform();
    const Matrix4& getWorldTransform() const;

    //ピクセル単位で位置指定
    void setPosition(const Vector2& pos);
    const Vector2& getPosition() const;
    void translate(const Vector2& translation);

    //回転
    void setRotation(float angle);
    float getRotation() const;
    void rotate(float angle);

    //ピボット位置
    void setPivot(Pivot pivot);
    const Vector2& getPivot() const;

    //倍率で拡縮
    void setScale(const Vector2& scale);
    void setScale(float scale);
    const Vector2& getScale() const;

    //ポリゴンサイズ
    void setSize(const Vector2& size);
    const Vector2& getSize() const;

private:
    void shouldRecomputeTransform();

private:
    Matrix4 mWorldTransform;
    Vector2 mPosition;
    float mRotation;
    Vector2 mPivot;
    Vector2 mScale;
    Vector2 mSize;
    bool mIsRecomputeTransform;
};

