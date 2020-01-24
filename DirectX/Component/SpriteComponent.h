#pragma once

#include "Component.h"
#include "../Sprite/Sprite.h"
#include "../Utility/Math.h"
#include <memory>

class Actor;
class Renderer;
class Sprite;

class SpriteComponent : public Component {
public:
    SpriteComponent(Actor* owner, const char* fileName);
    ~SpriteComponent();
    virtual void start() override;
    virtual void update() override;
    //色味、たぶん0～1
    void setColor(const Vector3& color);
    void setColor(float r, float g, float b);
    //不透明度(0～1、1が不透明, 0が透明)
    void setAlpha(float alpha);
    Vector4 getColor() const;
    //切り取り範囲(left, top, right, bottom, 0～1)
    void setUV(float l, float t, float r, float b);
    Vector4 getUV() const;
    //テクスチャサイズの取得
    Vector2 getTextureSize() const;
    //状態管理
    void setActive(bool value);
    bool getActive() const;
    SpriteState getState() const;
    //テクスチャ
    std::shared_ptr<Texture> texture() const;
    //シェーダーの取得
    std::shared_ptr<Shader> shader() const;
    //ファイル名の取得
    const char* fileName() const;

private:
    Sprite* mSprite;
};
