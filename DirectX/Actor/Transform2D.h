#pragma once

#include "../Utility/Math.h"
#include <memory>
#include <list>

class Transform2D : public std::enable_shared_from_this<Transform2D> {
public:
    Transform2D();
    ~Transform2D();

    //ワールド行列更新
    bool computeWorldTransform();
    const Matrix4& getWorldTransform() const;

    //ピクセル単位で位置指定
    void setPosition(const Vector2& pos);
    Vector2 getPosition() const;
    void translate(const Vector2& translation);

    //描画優先順位 基準50(1～100)
    void setPrimary(int primary);
    float getDepth() const;

    //回転
    void setRotation(float angle);
    const Quaternion& getRotation() const;
    void rotate(float angle);

    //回転縮小ピボット位置
    void setPivot(const Vector2& pivot);
    const Vector2& getPivot() const;

    //倍率で拡縮
    void setScale(const Vector2& scale);
    void setScale(float scale);
    const Vector2& getScale() const;

    //ポリゴンサイズ
    void setSize(const Vector2& size);
    const Vector2& getSize() const;

public:
    static bool zSortFlag;

private:
    void shouldRecomputeTransform();

private:
    Matrix4 mWorldTransform;
    Vector3 mPosition;
    Quaternion mRotation;
    Vector2 mPivot;
    Vector2 mScale;
    Vector2 mSize;
    bool mIsRecomputeTransform;

    static int mCount;
};

