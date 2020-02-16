#include "GBuffer.h"
#include "Buffer.h"
#include "BufferDesc.h"
#include "Game.h"
#include "ShaderResourceView.h"
#include "ShaderResourceViewDesc.h"
#include "SubResourceDesc.h"
#include "Texture2D.h"
#include "Texture2DDesc.h"
#include "Usage.h"
#include "../Device/Renderer.h"
#include "../Mesh/MeshLoader.h"
#include "../Shader/Shader.h"

GBuffer::GBuffer() :
    mSampler(nullptr),
    mShader(nullptr),
    mVertexBuffer(nullptr) {
}

GBuffer::~GBuffer() {
    for (auto&& rt : mRenderTargets) {
        SAFE_RELEASE(rt);
    }
    SAFE_RELEASE(mSampler);
}

void GBuffer::create(std::shared_ptr<Renderer> renderer) {
    //Deferred 関連 なおそれぞれ深度ステンシルを作るわけではない。サイズが同じなので通常のものを使い回せる。
    Texture2DDesc desc;
    ShaderResourceViewDesc srvDesc;
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
    ZeroMemory(&RTVDesc, sizeof(RTVDesc));

    //カラーマップ用テクスチャーとそのレンダーターゲットビュー、シェーダーリソースビューの作成
    desc.width = Game::WINDOW_WIDTH;
    desc.height = Game::WINDOW_HEIGHT;
    desc.format = Format::FORMAT_RGBA8_UNORM;
    desc.usage = Usage::USAGE_DEFAULT;
    desc.bindFlags = 
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_RENDER_TARGET) |
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_SHADER_RESOURCE);
    auto texture = renderer->createTexture2D(desc);

    RTVDesc.Format = toFormat(desc.format);
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.MipSlice = 0;
    ID3D11RenderTargetView* renderTarget;
    renderer->device()->CreateRenderTargetView(texture->texture2D(), &RTVDesc, &renderTarget);
    mRenderTargets.emplace_back(renderTarget);

    srvDesc.format = desc.format;
    mShaderResourceViews.emplace_back(std::make_shared<ShaderResourceView>(renderer, texture, &srvDesc));

    //ノーマルマップ用テクスチャーとそのレンダーターゲットビュー、シェーダーリソースビューの作成
    desc.format = Format::FORMAT_RGBA32_FLOAT;
    auto texture2 = renderer->createTexture2D(desc);

    RTVDesc.Format = toFormat(desc.format);
    renderer->device()->CreateRenderTargetView(texture2->texture2D(), &RTVDesc, &renderTarget);
    mRenderTargets.emplace_back(renderTarget);

    srvDesc.format = desc.format;
    mShaderResourceViews.emplace_back(std::make_shared<ShaderResourceView>(renderer, texture2, &srvDesc));

    //ポジションマップ用テクスチャーとそのレンダーターゲットビュー、シェーダーリソースビューの作成
    auto texture3 = renderer->createTexture2D(desc);

    renderer->device()->CreateRenderTargetView(texture3->texture2D(), &RTVDesc, &renderTarget);
    mRenderTargets.emplace_back(renderTarget);

    mShaderResourceViews.emplace_back(std::make_shared<ShaderResourceView>(renderer, texture3, &srvDesc));

    //サンプラー作成
    D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    renderer->device()->CreateSamplerState(&sd, &mSampler);

    //シェーダー生成
    mShader = renderer->createShader("Deferred.hlsl");
    mShader->createConstantBuffer(renderer, sizeof(GBufferShaderConstantBuffer));

    //バーテックスバッファ生成
    static const MeshVertex vertices[] = {
        Vector3(-1.f, -1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector2(0.f, 1.f),
        Vector3(-1.f, 1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector2(0.f, 0.f),
        Vector3(1.f, -1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector2(1.f, 1.f),
        Vector3(1.f, 1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector2(1.f, 0.f)
    };
    BufferDesc bd;
    bd.oneSize = sizeof(MeshVertex);
    bd.size = bd.oneSize * 4;
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);
    SubResourceDesc sub;
    sub.data = vertices;

    mVertexBuffer = renderer->createVertexBuffer(bd, &sub);
}

ID3D11RenderTargetView* GBuffer::getRenderTarget(unsigned index) const {
    return mRenderTargets[index];
}

std::shared_ptr<ShaderResourceView> GBuffer::getShaderResourceView(unsigned index) const {
    return mShaderResourceViews[index];
}

ID3D11SamplerState* GBuffer::getSampler() const {
    return mSampler;
}

std::shared_ptr<Shader> GBuffer::shader() const {
    return mShader;
}

std::shared_ptr<VertexBuffer> GBuffer::vertexBuffer() const {
    return mVertexBuffer;
}
