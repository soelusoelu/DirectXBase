#include "SpriteComponent.h"
#include "ComponentManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform2D.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../System/BlendDesc.h"
#include "../System/BlendState.h"
#include "../System/DepthStencilState.h"
#include "../System/DirectX.h"
#include "../System/Format.h"
#include "../System/IndexBuffer.h"
#include "../System/Texture.h"
#include "../System/VertexBuffer.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"

SpriteComponent::SpriteComponent() :
    Component(500),
    mSprite(nullptr) {
}

SpriteComponent::~SpriteComponent() {
    mSprite->destroy();
}

void SpriteComponent::onSetActive(bool value) {
    setActive(value);
}

void SpriteComponent::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    std::string str;
    if (JsonHelper::getString(inObj, "fileName", &str)) {
        setSprite(str);
    }
    bool isActive = true;
    if (JsonHelper::getBool(inObj, "isActive", &isActive)) {
        setActive(isActive);
    }
    Vector2 vec2;
    if (JsonHelper::getVector2(inObj, "position", &vec2)) {
        transform()->setPosition(vec2);
    }
    float value;
    if (JsonHelper::getFloat(inObj, "rotation", &value)) {
        transform()->setRotation(value);
    }
    if (JsonHelper::getVector2(inObj, "scale", &vec2)) {
        transform()->setScale(vec2);
    }
    Vector3 vec3;
    if (JsonHelper::getVector3(inObj, "color", &vec3)) {
        setColor(vec3);
    }
    if (JsonHelper::getFloat(inObj, "alpha", &value)) {
        setAlpha(value);
    }
    Vector4 vec4;
    if (JsonHelper::getVector4(inObj, "uv", &vec4)) {
        setUV(vec4.x, vec4.y, vec4.z, vec4.w);
    }
    if (JsonHelper::getString(inObj, "pivot", &str)) {
        Pivot pivot = Pivot::NONE;
        Transform2D::stringToPivot(str, &pivot);
        transform()->setPivot(pivot);
    }
}

void SpriteComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    Component::saveProperties(alloc, inObj);

    JsonHelper::setString(alloc, inObj, "fileName", fileName());
}

void SpriteComponent::drawDebugInfo(DebugInfoList* inspect) const {
    Component::drawDebugInfo(inspect);

    inspect->emplace_back("FileName", fileName());
    inspect->emplace_back("IsActive", getActive());
    inspect->emplace_back("Position", transform()->getPosition());
    inspect->emplace_back("Rotation", transform()->getRotation());
    inspect->emplace_back("Scale", transform()->getScale());
    inspect->emplace_back("Color", getColor());
    inspect->emplace_back("UV", getUV());
    inspect->emplace_back("TextureSize", getTextureSize());
}

void SpriteComponent::update() {
    mSprite->update();
}

void SpriteComponent::setSprite(const std::string& fileName) {
    mSprite = std::make_shared<Sprite>(fileName);
    addToManager();
}

const std::shared_ptr<Transform2D>& SpriteComponent::transform() const {
    return mSprite->transform();
}

void SpriteComponent::setColor(const Vector3& color) {
    mSprite->setColor(color);
}

void SpriteComponent::setColor(float r, float g, float b) {
    mSprite->setColor(r, g, b);
}

void SpriteComponent::setAlpha(float alpha) {
    mSprite->setAlpha(alpha);
}

const Vector4& SpriteComponent::getColor() const {
    return mSprite->getColor();
}

void SpriteComponent::setUV(float l, float t, float r, float b) {
    mSprite->setUV(l, t, r, b);
}

const Vector4& SpriteComponent::getUV() const {
    return mSprite->getUV();
}

const Vector2& SpriteComponent::getTextureSize() const {
    return mSprite->getTextureSize();
}

void SpriteComponent::setActive(bool value) {
    mSprite->setActive(value);
}

bool SpriteComponent::getActive() const {
    return mSprite->getActive();
}

bool SpriteComponent::isDead() const {
    return mSprite->isDead();
}

const Texture& SpriteComponent::texture() const {
    return mSprite->texture();
}

const Shader& SpriteComponent::shader() const {
    return mSprite->shader();
}

const std::string& SpriteComponent::fileName() const {
    return mSprite->fileName();
}

void SpriteComponent::draw() const {
    auto proj = Matrix4::identity;
    //原点をスクリーン左上にするために平行移動
    proj.m[3][0] = -1.f;
    proj.m[3][1] = 1.f;
    //ピクセル単位で扱うために
    proj.m[0][0] = 2.f / Window::width();
    proj.m[1][1] = -2.f / Window::height();

    //プリミティブ・トポロジーをセット
    Singleton<DirectX>::instance().setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_STRIP);
    //バーテックスバッファーをセット
    Texture::vertexBuffer->setVertexBuffer();
    //インデックスバッファーをセット
    Texture::indexBuffer->setIndexBuffer(Format::FORMAT_R16_UINT);
    //デプステスト無効化
    Singleton<DirectX>::instance().depthStencilState()->depthTest(false);
    //通常合成
    BlendDesc bd;
    bd.renderTarget.srcBlend = Blend::SRC_ALPHA;
    bd.renderTarget.destBlend = Blend::INV_SRC_ALPHA;
    Singleton<DirectX>::instance().blendState()->setBlendState(bd);

    mSprite->draw(proj);
}

void SpriteComponent::setSpriteManager(SpriteManager* manager) {
    mSpriteManager = manager;
}

void SpriteComponent::addToManager() {
    if (mSpriteManager) {
        mSpriteManager->addComponent(shared_from_this());
    }
}

SpriteManager* SpriteComponent::mSpriteManager = nullptr;
