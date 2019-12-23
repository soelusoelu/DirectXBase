#pragma once

#include "../Utility/Math.h"
#include <memory>

enum class SpriteState {
    ACTIVE, //��ɕ`��
    ONCE, //���`���폜
    DEAD //�`�悹���폜
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
    //SpriteManager�ɂĖ��t���[�����s
    void update();
    //�`��
    void draw(std::shared_ptr<Renderer> renderer, const Matrix4& proj);
    //���g�̃R�s�[���쐬
    std::shared_ptr<Sprite> copy() const;
    //�s�N�Z���P�ʂňʒu�w��
    void setPosition(const Vector2& pos);
    //�`��D�揇��(0�`1�A0����O)
    void setPrimary(float z);
    Vector2 getPosition() const;
    float getDepth() const;
    void translate(const Vector2& translation);
    //��]
    void setRotation(float angle);
    Quaternion getRotation() const;
    void rotate(float angle);
    //�{���Ŋg�k
    void setScale(const Vector2& scale, bool isCenterShift = false);
    void setScale(float scale, bool isCenterShift = false);
    Vector2 getScale() const;
    //�F���A���Ԃ�0�`1
    void setColor(const Vector3& color);
    void setColor(float r, float g, float b);
    //�s�����x(0�`1�A1���s����, 0������)
    void setAlpha(float alpha);
    Vector4 getColor() const;
    //�؂���͈�(left, top, right, bottom, 0�`1)
    void setUV(float l, float t, float r, float b);
    Vector4 getUV() const;
    //��]�s�{�b�g�ʒu
    void setPivot(const Vector2& pivot);
    Vector2 getPivot() const;
    //�e�N�X�`���T�C�Y�̎擾
    Vector2INT getTextureSize() const;
    //�e�N�X�`���̌��݂̃T�C�Y���擾
    Vector2INT getCurrentTextureSize() const;
    //�X�N���[���\����̃T�C�Y�̎擾
    Vector2INT getScreenTextureSize() const;
    //��ԊǗ�
    static void destroy(Sprite* sprite);
    static void destroy(std::shared_ptr<Sprite> sprite);
    SpriteState getState() const;
    //World�s��
    void setWorld(const Matrix4& world);
    Matrix4 getWorld() const;
    //�e�N�X�`���̎擾
    void setTexture(std::shared_ptr<Renderer> renderer, const char* fileName);
    std::shared_ptr<Texture> texture() const;
    //�V�F�[�_�[�̎擾
    std::shared_ptr<Shader> shader() const;
    //�t�@�C�����̎擾
    const char* fileName() const;
    //���[���h�s��̕ύX�t���O
    bool getWorldUpdateFlag() const;
    void onceToDead();

    //SpriteManager�̓o�^
    static void setSpriteManager(SpriteManager* manager);

private:
    void updateWorld();
    void centerShift(const Vector2& nextScale);

public:
    static bool mZSortFlag;

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

