﻿#pragma once

#include "../Utility/Math.h"
#include <memory>
#include <string>

class Renderer;
class Shader;
class SpriteManager;
class Texture;
class Transform2D;

class Sprite : public std::enable_shared_from_this<Sprite> {
    enum class State {
        ACTIVE,
        NON_ACTIVE,
        DEAD
    };

public:
    Sprite(std::shared_ptr<Renderer> renderer, const std::string& fileName);
    ~Sprite();
    //マネージャークラスへの登録
    void addToManager();
    //SpriteManagerにて毎フレーム実行
    void update();
    //描画
    void draw(const Matrix4& proj);
    //自身のコピーを作成
    std::shared_ptr<Sprite> copy() const;
    //Transform
    std::shared_ptr<Transform2D> transform() const;
    //色味、たぶん0～1
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
    void destroy();
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;
    void setOnceDraw();
    //テクスチャの取得
    std::shared_ptr<Texture> texture() const;
    //シェーダーの取得
    std::shared_ptr<Shader> shader() const;
    //ファイル名の取得
    const std::string& fileName() const;

    //SpriteManagerの登録
    static void setSpriteManager(SpriteManager* manager);

private:
    std::shared_ptr<Transform2D> mTransform;
    std::shared_ptr<Texture> mTexture;
    std::shared_ptr<Shader> mShader;
    Vector2 mTextureSize;
    Vector4 mColor;
    Vector4 mUV;
    State mState;
    std::string mFileName;
    bool mIsOnceDraw; //一回描画したら削除

    static SpriteManager* mSpriteManager;
};

