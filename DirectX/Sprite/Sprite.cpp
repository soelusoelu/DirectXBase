#include "Sprite.h"
#include "SpriteManager.h"
#include "Texture.h"
#include "../Device/Renderer.h"
#include "../Shader/Shader.h"
#include "../System/Buffer.h"
#include "../System/DirectXIncLib.h"
#include "../System/TextureDesc.h"
#include <cassert>

Sprite::Sprite(std::shared_ptr<Renderer> renderer, const char* fileName, float z, bool updateMyself) :
    mDefaultSize(Vector2INT::zero),
    mCurrentSize(Vector2INT::zero),
    mPosition(Vector2::zero, z),
    mRotation(Quaternion::identity),
    mScale(Vector2::one),
    mColor(ColorPalette::white, 1.f),
    mUV(0.f, 0.f, 1.f, 1.f),
    mPivot(Vector2::zero),
    mWorld(Matrix4::identity),
    mState(SpriteState::ACTIVE),
    mTexture(renderer->createTexture(fileName)),
    mShader(renderer->createShader("Texture.hlsl", "VS", "PS")),
    mFileName(fileName),
    mUpdateMyself(updateMyself),
    mWorldUpdateFlag(true) {

    auto desc = mTexture->desc();
    mDefaultSize = Vector2INT(desc.width, desc.height);
    mCurrentSize = mDefaultSize;
    mPivot = Vector2(mCurrentSize.x / 2.f, mCurrentSize.y / 2.f);

    mTexture->createInputLayout(renderer, mShader->getCompiledShader());

    if (mSpriteManager) {
        mSpriteManager->add(this);
    }
}

Sprite::~Sprite() = default;

Sprite::Sprite(const Sprite & sprite) :
    mDefaultSize(sprite.mDefaultSize),
    mCurrentSize(sprite.mCurrentSize),
    mPosition(sprite.mPosition),
    mRotation(sprite.mRotation),
    mScale(sprite.mScale),
    mColor(sprite.mColor),
    mUV(sprite.mUV),
    mPivot(sprite.mPivot),
    mWorld(sprite.mWorld),
    mState(SpriteState::ACTIVE),
    mTexture(sprite.mTexture),
    mShader(sprite.mShader),
    mFileName(sprite.mFileName),
    mUpdateMyself(sprite.mUpdateMyself),
    mWorldUpdateFlag(true) {
}

void Sprite::update() {
    updateWorld();
}

void Sprite::draw(std::shared_ptr<Renderer> renderer, const Matrix4& proj) {
    if (getState() == SpriteState::DEAD) {
        return;
    }
    onceToDead();

    //自身を使用するシェーダーとして登録
    mShader->setVSShader();
    mShader->setPSShader();
    //コンスタントバッファーを使うシェーダーの登録
    mShader->setVSConstantBuffers();
    mShader->setPSConstantBuffers();
    //頂点レイアウトをセット
    renderer->setInputLayout(mTexture->getVertexlayout());

    //シェーダーのコンスタントバッファーに各種データを渡す
    D3D11_MAPPED_SUBRESOURCE pData;
    if (SUCCEEDED(renderer->deviceContext()->Map(mShader->getConstantBuffer()->buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
        TextureShaderConstantBuffer cb;
        //ワールド、カメラ、射影行列を渡す
        cb.mWorld = mWorld;
        cb.mWorld.transpose();
        cb.mProjection = proj;
        cb.mProjection.transpose();
        cb.mColor = mColor;
        cb.mUV = mUV;
        memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
        renderer->deviceContext()->Unmap(mShader->getConstantBuffer()->buffer(), 0);
    }
    //テクスチャーをシェーダーに渡す
    mShader->setPSTextures(mTexture);
    //サンプラーのセット
    auto sample = mTexture->getSampler();
    renderer->deviceContext()->PSSetSamplers(0, 1, &sample);
    //プリミティブをレンダリング
    renderer->drawIndexed(6);
}

std::shared_ptr<Sprite> Sprite::copy() const {
    auto s = std::make_shared<Sprite>(*this);
    mSpriteManager->add(s);
    s->mState = SpriteState::ONCE;
    return s;
}

void Sprite::setPosition(const Vector2 & pos) {
    mPosition.x = pos.x;
    mPosition.y = pos.y;
    mWorldUpdateFlag = true;
}

void Sprite::setPrimary(float z) {
    mPosition.z = z;
    mWorldUpdateFlag = true;
    ZSortFlag = true;
}

Vector2 Sprite::getPosition() const {
    return Vector2(mPosition.x, mPosition.y);
}

float Sprite::getDepth() const {
    return mPosition.z;
}

void Sprite::translate(const Vector2 & translation) {
    mPosition.x += translation.x;
    mPosition.y += translation.y;
    mWorldUpdateFlag = true;
}

void Sprite::setRotation(float angle) {
    angle *= 0.5f;
    float sinAngle = Math::sin(angle);

    mRotation.z = sinAngle;
    mRotation.w = Math::cos(angle);

    mWorldUpdateFlag = true;
}

Quaternion Sprite::getRotation() const {
    return mRotation;
}

void Sprite::rotate(float angle) {
    angle *= 0.5f;
    float sinAngle = Math::sin(angle);

    Quaternion inc;
    inc.z = sinAngle;
    inc.w = Math::cos(angle);

    mRotation = Quaternion::concatenate(mRotation, inc);

    mWorldUpdateFlag = true;
}

void Sprite::setScale(const Vector2 & scale, bool isCenterShift) {
    if (isCenterShift) {
        centerShift(scale);
    }

    mScale = scale;

    //ピボット修正
    mPivot = Vector2(mCurrentSize.x / 2.f, mCurrentSize.y / 2.f);
    mPivot.x *= mScale.x;
    mPivot.y *= mScale.y;

    mWorldUpdateFlag = true;
}

void Sprite::setScale(float scale, bool isCenterShift) {
    if (isCenterShift) {
        centerShift(Vector2(scale, scale));
    }

    mScale.x = scale;
    mScale.y = scale;

    //ピボット修正
    mPivot = Vector2(mCurrentSize.x / 2.f, mCurrentSize.y / 2.f);
    mPivot.x *= mScale.x;
    mPivot.y *= mScale.y;

    mWorldUpdateFlag = true;
}

Vector2 Sprite::getScale() const {
    return mScale;
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

Vector4 Sprite::getColor() const {
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
    mCurrentSize.x = mDefaultSize.x * (r - l);
    mCurrentSize.y = mDefaultSize.y * (b - t);

    //ピボット修正
    mPivot = Vector2(mCurrentSize.x / 2.f, mCurrentSize.y / 2.f);
    mPivot.x *= mScale.x;
    mPivot.y *= mScale.y;

    mWorldUpdateFlag = true;
}

Vector4 Sprite::getUV() const {
    return mUV;
}

void Sprite::setPivot(const Vector2 & pivot) {
    mPivot = pivot;
    mWorldUpdateFlag = true;
}

Vector2 Sprite::getPivot() const {
    return mPivot;
}

Vector2INT Sprite::getTextureSize() const {
    return mDefaultSize;
}

Vector2INT Sprite::getCurrentTextureSize() const {
    return mCurrentSize;
}

Vector2INT Sprite::getScreenTextureSize() const {
    return Vector2INT(mCurrentSize.x * mScale.x, mCurrentSize.y * mScale.y);
}

void Sprite::destroy(Sprite * sprite) {
    sprite->mState = SpriteState::DEAD;
}

void Sprite::destroy(std::shared_ptr<Sprite> sprite) {
    sprite->mState = SpriteState::DEAD;
}

SpriteState Sprite::getState() const {
    return mState;
}

void Sprite::setWorld(const Matrix4 & world) {
    mWorld = world;
}

Matrix4 Sprite::getWorld() const {
    return mWorld;
}

void Sprite::setTexture(std::shared_ptr<Renderer> renderer, const char* fileName) {
    mTexture = renderer->createTexture(fileName);
    mFileName = fileName;

    auto desc = mTexture->desc();
    mDefaultSize = Vector2INT(desc.width, desc.height);
    mCurrentSize = mDefaultSize;
    mPivot = Vector2(mCurrentSize.x / 2.f, mCurrentSize.y / 2.f);

    mTexture->createInputLayout(renderer, mShader->getCompiledShader());
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

bool Sprite::getWorldUpdateFlag() const {
    return mWorldUpdateFlag;
}

void Sprite::onceToDead() {
    if (mState == SpriteState::ONCE) {
        mState = SpriteState::DEAD;
    }
}

void Sprite::setSpriteManager(SpriteManager* manager) {
    mSpriteManager = manager;
}

void Sprite::updateWorld() {
    //ワールド行列に変更が生じたら
    if (!mWorldUpdateFlag || !mUpdateMyself) {
        return;
    }
    mWorldUpdateFlag = false;

    mWorld = Matrix4::createScale(Vector3(getScreenTextureSize().x, getScreenTextureSize().y, 1.f));
    mWorld *= Matrix4::createTranslation(Vector3(-mPivot, 0.f));
    mWorld *= Matrix4::createFromQuaternion(mRotation);
    mWorld *= Matrix4::createTranslation(mPosition + Vector3(mPivot, 0.f));
}

void Sprite::centerShift(const Vector2 & nextScale) {
    auto PreviosSize = Vector2(getScreenTextureSize().x, getScreenTextureSize().y);
    auto nextSize = Vector2(mCurrentSize.x * nextScale.x, mCurrentSize.y * nextScale.y);
    auto shift = (PreviosSize - nextSize) / 2.f;

    translate(shift);
}

bool Sprite::ZSortFlag = false;
SpriteManager* Sprite::mSpriteManager = nullptr;
