#include "Sprite3D.h"
#include "../Sprite/SpriteManager.h"
#include "../DebugLayer/Debug.h"
#include "../Device/AssetsManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Shader/Shader.h"
#include "../System/DirectX.h"
#include "../System/Texture.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"
#include <cassert>

Sprite3D::Sprite3D() :
    Component(500),
    mTransform(std::make_shared<Transform3D>()),
    mTexture(nullptr),
    mShader(nullptr),
    mState(State::ACTIVE),
    mTextureAspect(Vector2::zero),
    mColor(Vector4(1.f, 1.f, 1.f, 1.f)),
    mUV(Vector4(0.f, 0.f, 1.f, 1.f)),
    mFileName("") {
}

Sprite3D::~Sprite3D() {
    mState = State::DEAD;
}

void Sprite3D::start() {
    if (mFileName.empty()) {
        Debug::logWarning(getComponentName() + ": No filename.");
        return;
    }

    mTexture = Singleton<AssetsManager>::instance().createTexture(mFileName);
    mShader = Singleton<AssetsManager>::instance().createShader("Texture.hlsl");

    //デスクをもとにテクスチャのアスペクト比を取得
    auto desc = mTexture->desc();
    auto v = static_cast<float>(desc.height) / static_cast<float>(desc.width);
    mTextureAspect.set(1.f, v);
    mTransform->setScale(mTransform->getScale() * Vector3(mTextureAspect, 1.f));

    mShader->createConstantBuffer(sizeof(TextureConstantBuffer));

    //インプットレイアウトの生成
    static constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    mShader->createInputLayout(layout, numElements);

    addToManager();
}

void Sprite3D::update() {
    mTransform->computeWorldTransform();
}

void Sprite3D::onSetActive(bool value) {
    setActive(value);
}

void Sprite3D::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "fileName", &mFileName);
    bool isActive = true;
    if (JsonHelper::getBool(inObj, "isActive", &isActive)) {
        setActive(isActive);
    }
    Vector3 vec3;
    if (JsonHelper::getVector3(inObj, "position", &vec3)) {
        mTransform->setPosition(vec3);
    }
    if (JsonHelper::getVector3(inObj, "rotation", &vec3)) {
        mTransform->rotate(vec3);
    }
    Vector2 vec2;
    if (JsonHelper::getVector2(inObj, "scale", &vec2)) {
        mTransform->setScale(Vector3(vec2, 1.f));
    }
    if (JsonHelper::getVector3(inObj, "color", &vec3)) {
        setColor(vec3);
    }
    float alpha;
    if (JsonHelper::getFloat(inObj, "alpha", &alpha)) {
        setAlpha(alpha);
    }
    Vector4 vec4;
    if (JsonHelper::getVector4(inObj, "uv", &vec4)) {
        setUV(vec4.x, vec4.y, vec4.z, vec4.w);
    }
}

void Sprite3D::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FileName", mFileName);
    inspect->emplace_back("IsActive", getActive());
    inspect->emplace_back("Position", mTransform->getPosition());
    inspect->emplace_back("Rotation", mTransform->getRotation().euler());
    inspect->emplace_back("Scale", mTransform->getScale());
    inspect->emplace_back("Color", mColor);
    inspect->emplace_back("UV", mUV);
}

void Sprite3D::draw(const Matrix4& viewProj) const {
    //シェーダーを登録
    mShader->setVSShader();
    mShader->setPSShader();
    //コンスタントバッファーを使うシェーダーの登録
    mShader->setVSConstantBuffers();
    mShader->setPSConstantBuffers();
    //頂点レイアウトをセット
    mShader->setInputLayout();

    //シェーダーのコンスタントバッファーに各種データを渡す
    MappedSubResourceDesc msrd;
    if (mShader->map(&msrd)) {
        TextureConstantBuffer cb;
        //ワールド、射影行列を渡す
        cb.wp = mTransform->getWorldTransform() * viewProj;
        cb.wp.transpose();
        cb.color = mColor;
        cb.uv = mUV;

        memcpy_s(msrd.data, msrd.rowPitch, (void*)(&cb), sizeof(cb));
        mShader->unmap();
    }
    //テクスチャーをシェーダーに渡す
    mTexture->setPSTextures();
    //サンプラーのセット
    mTexture->setPSSamplers();
    //プリミティブをレンダリング
    Singleton<DirectX>::instance().drawIndexed(6);
}

const std::shared_ptr<Transform3D>& Sprite3D::transform() const {
    return mTransform;
}

void Sprite3D::setColor(const Vector3& color) {
    mColor.x = color.x;
    mColor.y = color.y;
    mColor.z = color.z;
}

void Sprite3D::setColor(float r, float g, float b) {
    mColor.x = r;
    mColor.y = g;
    mColor.z = b;
}

void Sprite3D::setAlpha(float alpha) {
    mColor.w = alpha;
}

const Vector4& Sprite3D::getColor() const {
    return mColor;
}

void Sprite3D::setUV(float l, float t, float r, float b) {
    assert(0.f <= l || l <= 1.f);
    assert(0.f <= t || t <= 1.f);
    assert(l <= r || r <= 1.f);
    assert(t <= b || b <= 1.f);

    mUV.x = l;
    mUV.y = t;
    mUV.z = r;
    mUV.w = b;
}

const Vector4& Sprite3D::getUV() const {
    return mUV;
}

void Sprite3D::setActive(bool value) {
    mState = (value) ? State::ACTIVE : State::NON_ACTIVE;
}

bool Sprite3D::getActive() const {
    return mState == State::ACTIVE;
}

bool Sprite3D::isDead() const {
    return mState == State::DEAD;
}

const Texture& Sprite3D::texture() const {
    return *mTexture;
}

const Shader& Sprite3D::shader() const {
    return *mShader;
}

const std::string& Sprite3D::fileName() const {
    return mFileName;
}

void Sprite3D::setSpriteManager(SpriteManager* manager) {
    mSpriteManager = manager;
}

void Sprite3D::addToManager() {
    if (mSpriteManager) {
        mSpriteManager->add3D(shared_from_this());
    }
}

SpriteManager* Sprite3D::mSpriteManager = nullptr;
