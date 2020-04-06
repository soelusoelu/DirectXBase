#include "Renderer.h"
#include "DrawString.h"
#include "Sound.h"
#include "../Component/PointLightComponent.h"
#include "../Light/PointLight.h"
#include "../Mesh/IMeshLoader.h"
#include "../Mesh/VertexArray.h"
#include "../Shader/Shader.h"
#include "../System/BlendState.h"
#include "../System/DirectX.h"
#include "../System/DepthStencilState.h"
#include "../System/Format.h"
#include "../System/GBuffer.h"
#include "../System/IndexBuffer.h"
#include "../System/RenderTargetView.h"
#include "../System/RenderTargetViewDesc.h"
#include "../System/Sampler.h"
#include "../System/ShaderResourceView.h"
#include "../System/SubResourceDesc.h"
#include "../System/RasterizerState.h"
#include "../System/Texture.h"
#include "../System/Texture2D.h"
#include "../System/VertexBuffer.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"

Renderer::Renderer() :
    mDrawString(std::make_shared<DrawString>()),
    mGBuffer(std::make_unique<GBuffer>()),
    mAmbientLight(Vector3::zero),
    mPointLight(std::make_shared<PointLight>()) {
}

Renderer::~Renderer() = default;

void Renderer::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getVector3(inObj, "ambientLight", &mAmbientLight);
    mDrawString->loadProperties(inObj);
}

void Renderer::initialize() {
    mGBuffer->create();
    mPointLight->initialize();
    mDrawString->initialize();
}

void Renderer::update() {
    removePointLight();
}

std::shared_ptr<DrawString> Renderer::getDrawString() const {
    return mDrawString;
}

const Vector3& Renderer::getAmbientLight() const {
    return mAmbientLight;
}

void Renderer::setAmbientLight(const Vector3 & ambient) {
    mAmbientLight = ambient;
}

void Renderer::addPointLight(const std::shared_ptr<PointLightComponent>& light) {
    mPointLights.emplace_back(light);
}

void Renderer::drawPointLights() {
    if (mPointLights.empty()) {
        return;
    }

    auto shader = mPointLight->shader;
    //使用するシェーダーの登録
    shader->setVSShader();
    shader->setPSShader();
    //このコンスタントバッファーを使うシェーダーの登録
    shader->setVSConstantBuffers();
    shader->setPSConstantBuffers();
    //頂点インプットレイアウトをセット
    shader->setInputLayout();
    //バーテックスバッファーをセット
    mPointLight->mesh->getVertexArray()->setVertexBuffer();
    //プリミティブ指定
    Singleton<DirectX>::instance().setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_LIST);
    //テクスチャ登録
    mGBuffer->setPSShaderResources();
    //サンプラーをセット
    //mGBuffer->getSampler()->setPSSamplers();
    //デプステスト有効化
    Singleton<DirectX>::instance().depthStencilState()->depthTest(true);
    //デプスマスク無効化
    Singleton<DirectX>::instance().depthStencilState()->depthMask(false);
    //加算合成
    BlendDesc bd;
    bd.renderTarget.srcBlend = Blend::ONE;
    bd.renderTarget.destBlend = Blend::ONE;
    Singleton<DirectX>::instance().blendState()->setBlendState(bd);

    for (auto&& pointLight : mPointLights) {
        if (auto p = pointLight.lock()) {
            p->draw(*mPointLight);
        }
    }
}

void Renderer::renderToTexture() {
    Singleton<DirectX>::instance().setViewport(Window::width(), Window::height());
    mGBuffer->renderToTexture();
}

void Renderer::renderFromTexture(const Camera& camera, const DirectionalLight& dirLight) {
    mGBuffer->renderFromTexture(camera, dirLight, mAmbientLight);
}

void Renderer::renderSprite(Matrix4* proj) {
    //プロジェクション
    //原点をスクリーン左上にするために平行移動
    proj->m[3][0] = -1.f;
    proj->m[3][1] = 1.f;
    //ピクセル単位で扱うために
    proj->m[0][0] = 2.f / Window::width();
    proj->m[1][1] = -2.f / Window::height();

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
}

void Renderer::renderToDebug(Matrix4* proj) {
    Singleton<DirectX>::instance().setDebugRenderTarget();
    Singleton<DirectX>::instance().setViewport(Window::debugWidth(), Window::debugHeight());

    //原点をスクリーン左上にするために平行移動
    proj->m[3][0] = -1.f;
    proj->m[3][1] = 1.f;
    //ピクセル単位で扱うために
    proj->m[0][0] = 2.f / Window::width();
    proj->m[1][1] = -2.f / Window::height();
}

void Renderer::removePointLight() {
    if (mPointLights.empty()) {
        return;
    }

    auto itr = mPointLights.begin();
    while (itr != mPointLights.end()) {
        if (itr->expired()) {
            itr = mPointLights.erase(itr);
        } else {
            ++itr;
        }
    }
}
