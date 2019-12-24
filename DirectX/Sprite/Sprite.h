#pragma once

#include "../Utility/Math.h"
#include <memory>

enum class SpriteState {
    ACTIVE, //常に描画
    ONCE, //一回描画後削除
    DEAD //描画せず削除
};

class SpriteManager;
class Shader;
class Texture;
class Renderer;

class Sprite {
public:
    Sprite(std::shared_ptr<Renderer> renderer, const char* fileName, float z, bool updateMyself = true);
    ~Sprite();
    Sprite(const Sprite& sprite);
    //SpriteManagerにて毎フレーム実行
    void update();
    //描画
    void draw(std::shared_ptr<Renderer> renderer, const Matrix4& proj);
    //自身のコピーを作成
    std::shared_ptr<Sprite> copy() const;
    //ピクセル単位で位置指定
    void setPosition(const Vector2& pos);
    //描画優先順位(0～1、0が手前)
    void setPrimary(float z);
    Vector2 getPosition() const;
    float getDepth() const;
    void translate(const Vector2& translation);
    //回転
    void setRotation(float angle);
    Quaternion getRotation() const;
    void rotate(float angle);
    //倍率で拡縮
    void setScale(const Vector2& scale, bool isCenterShift = false);
    void setScale(float scale, bool isCenterShift = false);
    Vector2 getScale() const;
    //色味、たぶん0～1
    void setColor(const Vector3& color);
    void setColor(float r, float g, float b);
    //不透明度(0～1、1が不透明, 0が透明)
    void setAlpha(float alpha);
    Vector4 getColor() const;
    //切り取り範囲(left, top, right, bottom, 0～1)
    void setUV(float l, float t, float r, float b);
    Vector4 getUV() const;
    //回転ピボット位置
    void setPivot(const Vector2& pivot);
    Vector2 getPivot() const;
    //テクスチャサイズの取得
    Vector2INT getTextureSize() const;
    //テクスチャの現在のサイズを取得
    Vector2INT getCurrentTextureSize() const;
    //スクリーン表示上のサイズの取得
    Vector2INT getScreenTextureSize() const;
    //状態管理
    static void destroy(Sprite* sprite);
    static void destroy(std::shared_ptr<Sprite> sprite);
    SpriteState getState() const;
    //World行列
    void setWorld(const Matrix4& world);
    Matrix4 getWorld() const;
    //テクスチャの取得
    void setTexture(std::shared_ptr<Renderer> renderer, const char* fileName);
    std::shared_ptr<Texture> texture() const;
    //シェーダーの取得
    std::shared_ptr<Shader> shader() const;
    //ファイル名の取得
    const char* fileName() const;
    //ワールド行列の変更フラグ
    bool getWorldUpdateFlag() const;
    void onceToDead();

    //SpriteManagerの登録
    static void setSpriteManager(SpriteManager* manager);

private:
    void updateWorld();
    void centerShift(const Vector2& nextScale);

public:
    static bool ZSortFlag;

private:
    Vector2INT mDefaultSize;
    Vector2INT mCurrentSize;
    Vector3 mPosition;
    Quaternion mRotation;
    Vector2 mScale;
    Vector4 mColor;
    Vector4 mUV;
    Vector2 mPivot;
    Matrix4 mWorld;
    SpriteState mState;
    std::shared_ptr<Texture> mTexture;
    std::shared_ptr<Shader> mShader;
    const char* mFileName;
    bool mUpdateMyself;
    bool mWorldUpdateFlag;

    static SpriteManager* mSpriteManager;
};

