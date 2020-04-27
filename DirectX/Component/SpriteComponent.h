#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include "../Sprite/Sprite.h"
#include <memory>
#include <string>

class Sprite;
class Transform2D;

class SpriteComponent : public Component {
public:
    SpriteComponent(std::shared_ptr<GameObject> owner);
    ~SpriteComponent();
    virtual void onSetActive(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const;
    void setSprite(const std::string& fileName);
    //トランスフォーム
    std::shared_ptr<Transform2D> transform() const;
    //色味
    void setColor(const Vector3& color);
    void setColor(float r, float g, float b);
    //不透明度(0～1、1が不透明, 0が透明)
    void setAlpha(float alpha);
    const Vector4& getColor() const;
    //切り取り範囲(left, top, right, bottom, 0～1)
    void setUV(float l, float t, float r, float b);
    const Vector4& getUV() const;
    //テクスチャサイズの取得
    const Vector2& getTextureSize() const;
    //状態管理
    void setActive(bool value);
    bool getActive() const;
    //テクスチャ
    const Texture& texture() const;
    //シェーダーの取得
    const Shader& shader() const;

private:
    std::shared_ptr<Sprite> mSprite;
};

