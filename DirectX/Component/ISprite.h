#pragma once

#include <memory>
#include <string>

class Vector2;
class Vector3;
class Vector4;
class Transform2D;
class Texture;
class Shader;

class ISprite {
public:
    virtual ~ISprite() = default;
    //毎フレーム更新
    virtual void update() = 0;
    //スプライトを生成しセット
    virtual void setSprite(const std::string& fileName) = 0;
    //トランスフォーム
    virtual const std::shared_ptr<Transform2D>& transform() const = 0;
    //色味
    virtual void setColor(const Vector3& color) = 0;
    virtual void setColor(float r, float g, float b) = 0;
    //不透明度(0～1、1が不透明, 0が透明)
    virtual void setAlpha(float alpha) = 0;
    virtual const Vector4& getColor() const = 0;
    //切り取り範囲(left, top, right, bottom, 0～1)
    virtual void setUV(float l, float t, float r, float b) = 0;
    virtual const Vector4& getUV() const = 0;
    //テクスチャサイズの取得
    virtual const Vector2& getTextureSize() const = 0;
    //状態管理
    virtual void setActive(bool value) = 0;
    virtual bool getActive() const = 0;
    virtual bool isDead() const = 0;
    //テクスチャ
    virtual const Texture& texture() const = 0;
    //シェーダーの取得
    virtual const Shader& shader() const = 0;
    //ファイル名の取得
    virtual const std::string& fileName() const = 0;
};