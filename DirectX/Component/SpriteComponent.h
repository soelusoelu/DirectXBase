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
    //�g�����X�t�H�[��
    std::shared_ptr<Transform2D> transform() const;
    //�F��
    void setColor(const Vector3& color);
    void setColor(float r, float g, float b);
    //�s�����x(0�`1�A1���s����, 0������)
    void setAlpha(float alpha);
    const Vector4& getColor() const;
    //�؂���͈�(left, top, right, bottom, 0�`1)
    void setUV(float l, float t, float r, float b);
    const Vector4& getUV() const;
    //�e�N�X�`���T�C�Y�̎擾
    const Vector2& getTextureSize() const;
    //��ԊǗ�
    void setActive(bool value);
    bool getActive() const;
    //�e�N�X�`��
    const Texture& texture() const;
    //�V�F�[�_�[�̎擾
    const Shader& shader() const;

private:
    std::shared_ptr<Sprite> mSprite;
};

