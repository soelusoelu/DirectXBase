#pragma once

#include "Component.h"
#include <memory>

class Camera;
class Shader;
class SpriteManager;
class Texture;
class Transform3D;

class Billboard : public Component {
    enum class State {
        ACTIVE,
        NON_ACTIVE,
        DEAD
    };

public:
    Billboard(std::shared_ptr<GameObject> owner);
    ~Billboard();
//    virtual void start() override;
//
//    //SpriteManagerにて毎フレーム実行
//    void update();
//    //描画
//    void draw(const Matrix4& proj) const;
//    //色味、たぶん0～1
//    void setColor(const Vector3& color);
//    void setColor(float r, float g, float b);
//    //不透明度(0～1、1が不透明, 0が透明)
//    void setAlpha(float alpha);
//    const Vector4& getColor() const;
//    //切り取り範囲(left, top, right, bottom, 0～1)
//    void setUV(float l, float t, float r, float b);
//    const Vector4& getUV() const;
//    //テクスチャサイズの取得
//    const Vector2& getTextureSize() const;
//    //状態管理
//    void destroy();
//    void setActive(bool value);
//    bool getActive() const;
//    bool isDead() const;
//    //テクスチャの取得
//    const Texture& texture() const;
//    //シェーダーの取得
//    const Shader& shader() const;
//    //ファイル名の取得
//    const std::string& fileName() const;
//
//    //SpriteManagerの登録
//    static void setSpriteManager(SpriteManager* manager);
//
//private:
//    std::shared_ptr<Texture> mTexture;
//    std::shared_ptr<Shader> mShader;
//    Vector2 mTextureSize;
//    Vector4 mColor;
//    Vector4 mUV;
//    State mState;
//    std::string mFileName;
//
//    static SpriteManager* mSpriteManager;
};
