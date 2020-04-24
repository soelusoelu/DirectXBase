#pragma once

#include <memory>

class Vector3;
struct Material;

class IMesh {
public:
    virtual ~IMesh() = default;
    //カメラに写っているか
    virtual bool isVisible() const = 0;
    //マテリアル数の取得
    virtual size_t getNumMaterial() const = 0;
    //マテリアルの取得
    virtual std::shared_ptr<Material> getMaterial(unsigned index) const = 0;
    //中心座標の取得
    virtual const Vector3& getCenter() const = 0;
    //半径の取得
    virtual float getRadius() const = 0;
    //全体の色合い(シェーダー側で使用している必要あり)
    virtual void setColor(const Vector3& color) = 0;
    virtual const Vector3& getColor() const = 0;
    //状態
    virtual void destroy() = 0;
    virtual void setActive(bool value) = 0;
    virtual bool getActive() const = 0;
    virtual bool isDead() const = 0;
};
