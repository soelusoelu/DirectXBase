#include "Sprite.h"
#include "SpriteManager.h"
#include "Texture.h"
#include "../Actor/Transform2D.h"
#include "../Device/Renderer.h"
#include "../Shader/Shader.h"
#include "../System/Buffer.h"
#include "../System/InputElementDesc.h"
#include "../System/TextureDesc.h"
#include "../System/VertexStreamDesc.h"
#include <cassert>

Sprite::Sprite(std::shared_ptr<Renderer> renderer, const char* fileName) :
    mRenderer(renderer),
    mTransform(std::make_shared<Transform2D>()),
    mTexture(mRenderer->createTexture(fileName)),
    mShader(mRenderer->createShader("Texture.hlsl")),
    mTextureSize(Vector2::zero),
    mColor(ColorPalette::white, 1.f),
    mUV(0.f, 0.f, 1.f, 1.f),
    mState(SpriteState::ACTIVE),
    mFileName(fileName) {

    //デスクをもとにサイズ取得
    auto desc = mTexture->desc();
    mTextureSize = Vector2(desc.width, desc.height);

    //Transformに通知
    mTransform->setSize(mTextureSize);

    mShader->createConstantBuffer(mRenderer, sizeof(TextureShaderConstantBuffer), 0);

    //インプットレイアウトの生成
    constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    mShader->createInputLayout(layout, numElements);

    if (mSpriteManager) {
        mSpriteManager->add(this);
    }
}

Sprite::~Sprite() = default;

Sprite::Sprite(const Sprite & sprite) :
    mTransform(sprite.mTransform),
    mTextureSize(sprite.mTextureSize),
    mTexture(sprite.mTexture),
    mShader(sprite.mShader),
    mColor(sprite.mColor),
    mUV(sprite.mUV),
    mState(SpriteState::ACTIVE),
    mFileName(sprite.mFileName) {
}

void Sprite::update() {
    if (getActive()) {
        mTransform->computeWorldTransform();
    }
}

void Sprite::draw(const Matrix4 & proj) {
    if (!getActive() || isDead()) {
        return;
    }

    //シェーダーを登録
    mShader->setVSShader();
    mShader->setPSShader();
    //コンスタントバッファーを使うシェーダーの登録
    mShader->setVSConstantBuffers();
    mShader->setPSConstantBuffers();
    //頂点レイアウトをセット
    mShader->setInputLayout();

    //シェーダーのコンスタントバッファーに各種データを渡す
    D3D11_MAPPED_SUBRESOURCE pData;
    if (mShader->map(&pData)) {
        TextureShaderConstantBuffer cb;
        //ワールド、射影行列を渡す
        cb.mWorld = mTransform->getWorldTransform();
        cb.mWorld.transpose();
        cb.mProjection = proj;
        cb.mProjection.transpose();
        cb.mColor = mColor;
        cb.mUV = mUV;

        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        mShader->unmap();
    }
    //テクスチャーをシェーダーに渡す
    mTexture->setPSTextures();
    //サンプラーのセット
    mTexture->setPSSamplers();
    //プリミティブをレンダリング
    mRenderer->drawIndexed(6);
}

std::shared_ptr<Sprite> Sprite::copy() const {
    auto s = std::make_shared<Sprite>(*this);
    mSpriteManager->add(s);
    return s;
}

std::shared_ptr<Transform2D> Sprite::transform() const {
    return mTransform;
}

void Sprite::setColor(const Vector3 & color) {
    mColor.x = color.x;
    mColor.y = color.y;
    mColor.z = color.z;
}

void Sprite::setColor(float r, float g, float b) {
    mColor.x = r;
    mColor.y = g;
    mColor.z = b;
}

void Sprite::setAlpha(float alpha) {
    mColor.w = alpha;
}

const Vector4& Sprite::getColor() const {
    return mColor;
}

void Sprite::setUV(float l, float t, float r, float b) {
    assert(0.f <= l || l <= 1.f);
    assert(0.f <= t || t <= 1.f);
    assert(l <= r || r <= 1.f);
    assert(t <= b || b <= 1.f);

    mUV.x = l;
    mUV.y = t;
    mUV.z = r;
    mUV.w = b;

    //サイズ修正
    Vector2 size;
    size.x = mTextureSize.x * (r - l);
    size.y = mTextureSize.y * (b - t);

    //テクスチャサイズを変更したことを通知
    mTransform->setSize(size);
}

const Vector4& Sprite::getUV() const {
    return mUV;
}

const Vector2& Sprite::getTextureSize() const {
    return mTextureSize;
}

void Sprite::destroy() {
    mState = SpriteState::DEAD;
}

void Sprite::setActive(bool value) {
    mState = (value) ? SpriteState::ACTIVE : SpriteState::NON_ACTIVE;
}

bool Sprite::getActive() const {
    return mState == SpriteState::ACTIVE;
}

bool Sprite::isDead() const {
    return mState == SpriteState::DEAD;
}

std::shared_ptr<Texture> Sprite::texture() const {
    return mTexture;
}

std::shared_ptr<Shader> Sprite::shader() const {
    return mShader;
}

const char* Sprite::fileName() const {
    return mFileName;
}

void Sprite::setSpriteManager(SpriteManager * manager) {
    mSpriteManager = manager;
}

SpriteManager* Sprite::mSpriteManager = nullptr;
