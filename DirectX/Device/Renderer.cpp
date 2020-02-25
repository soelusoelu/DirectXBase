#include "Renderer.h"
#include "AssetsManager.h"
#include "Sound.h"
#include "../Camera/Camera.h"
#include "../Component/PointLightComponent.h"
#include "../Light/DirectionalLight.h"
#include "../Light/PointLight.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/MeshLoader.h"
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
#include "../Utility/LevelLoader.h"

Renderer::Renderer() :
    mAssetsManager(std::make_shared<AssetsManager>()),
    mGBuffer(std::make_unique<GBuffer>()),
    mAmbientLight(Vector3::zero),
    mDirectionalLight(std::make_shared<DirectionalLight>()),
    mPointLight(std::make_shared<PointLight>()) {
}

Renderer::~Renderer() = default;

void Renderer::initialize() {
    mGBuffer->create(shared_from_this());
    mPointLight->initialize(shared_from_this());

    //ディレクショナルライトと環境光の読み込み
    Singleton<LevelLoader>::instance().loadGlobal(shared_from_this(), "GlobalLight.json");
}

void Renderer::update() {
    mDirectionalLight->update();
    removePointLight();
}

std::shared_ptr<AssetsManager> Renderer::getAssetsManager() const {
    return mAssetsManager;
}

const Vector3& Renderer::getAmbientLight() const {
    return mAmbientLight;
}

void Renderer::setAmbientLight(const Vector3 & ambient) {
    mAmbientLight = ambient;
}

std::shared_ptr<DirectionalLight> Renderer::getDirectionalLight() const {
    return mDirectionalLight;
}

void Renderer::addPointLight(std::shared_ptr<PointLightComponent> light) {
    mPointLights.emplace_back(light);
}

void Renderer::drawPointLights(std::shared_ptr<Camera> camera) {
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
    mPointLight->mesh->getMeshData()->setVertexBuffer();
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
            p->draw(mPointLight, camera);
        }
    }
}

void Renderer::renderToTexture() {
    mGBuffer->renderToTexture();
}

void Renderer::renderFromTexture(std::shared_ptr<Camera> camera) {
    mGBuffer->renderFromTexture(camera, mDirectionalLight, mAmbientLight);
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
