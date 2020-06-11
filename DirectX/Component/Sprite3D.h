﻿#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <memory>

class Shader;
class SpriteManager;
class Texture;
class Transform3D;

class Sprite3D : public Component, public std::enable_shared_from_this<Sprite3D> {
public:
    Sprite3D();
    ~Sprite3D();
    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void finalize() override;
    virtual void onSetActive(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

    //描画
    void draw(const Matrix4& viewProj) const;
    //トランスフォーム
    const std::shared_ptr<Transform3D>& transform() const;
    //色味、たぶん0～1
    void setColor(const Vector3& color);
    void setColor(float r, float g, float b);
    //不透明度(0～1、1が不透明, 0が透明)
    void setAlpha(float alpha);
    //色味の取得
    const Vector4& getColor() const;
    //切り取り範囲(left, top, right, bottom, 0～1)
    void setUV(float l, float t, float r, float b);
    const Vector4& getUV() const;
    //状態管理
    void setActive(bool value);
    bool getActive() const;
    //テクスチャの取得
    const Texture& texture() const;
    //シェーダーの取得
    const Shader& shader() const;
    //ファイル名の取得
    const std::string& fileName() const;

    //SpriteManagerの登録
    static void setSpriteManager(SpriteManager* manager);

protected:
    //マネージャークラスへの登録
    void addToManager();

protected:
    std::shared_ptr<Transform3D> mTransform;
    std::shared_ptr<Texture> mTexture;
    std::shared_ptr<Shader> mShader;
    Vector2 mTextureAspect;
    Vector4 mColor;
    Vector4 mUV;
    std::string mFileName;
    bool mIsActive;

    static inline SpriteManager* mSpriteManager = nullptr;
};
