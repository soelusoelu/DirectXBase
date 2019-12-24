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
    SpriteComponent(Actor* owner, std::shared_ptr<Renderer> renderer, const char* fileName, float z);
    ~SpriteComponent();
    virtual void start() override;
    virtual void update() override;
    Sprite* getSprite() const;

    //�`��D�揇��(0�`1�A0����O)
    void setPrimary(float z);
    float getDepth() const;
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
    SpriteState getState() const;
    //World�s��
    void setWorld(const Matrix4& world);
    Matrix4 getWorld() const;
    //�e�N�X�`��
    void setTexture(std::shared_ptr<Renderer> renderer, const char* fileName);
    std::shared_ptr<Texture> texture() const;
    //�V�F�[�_�[�̎擾
    std::shared_ptr<Shader> shader() const;
    //�t�@�C�����̎擾
    const char* fileName() const;

private:
    Sprite* mSprite;
};
