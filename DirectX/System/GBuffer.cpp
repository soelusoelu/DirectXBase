#include "GBuffer.h"
#include "Buffer.h"
#include "BufferDesc.h"
#include "Game.h"
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
    for (auto&& sr : mShaderResources) {
        SAFE_RELEASE(sr);
    }
    SAFE_RELEASE(mSampler);
}

void GBuffer::create(std::shared_ptr<Renderer> renderer) {
    //Deferred 関連 なおそれぞれ深度ステンシルを作るわけではない。サイズが同じなので通常のものを使い回せる。
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
    Texture2DDesc desc;
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));
    ZeroMemory(&RTVDesc, sizeof(RTVDesc));

    //カラーマップ用テクスチャーとそのレンダーターゲットビュー、シェーダーリソースビューの作成
    desc.width = Game::WINDOW_WIDTH;
    desc.height = Game::WINDOW_HEIGHT;
    desc.format = Format::FORMAT_RGBA8_UNORM;
    desc.usage = Usage::USAGE_DEFAULT;
    desc.bindFlags = 
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_RENDER_TARGET) |
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_SHADER_RESOURCE);
    auto texture = renderer->texture2D()->createTexture2D(desc, nullptr);

    RTVDesc.Format = toFormat(desc.format);
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.MipSlice = 0;
    ID3D11RenderTargetView* renderTarget;
    renderer->device()->CreateRenderTargetView(texture, &RTVDesc, &renderTarget);
    mRenderTargets.emplace_back(renderTarget);

    SRVDesc.Format = toFormat(desc.format);
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;
    ID3D11ShaderResourceView* shaderResource;
    renderer->device()->CreateShaderResourceView(texture, &SRVDesc, &shaderResource);
    mShaderResources.emplace_back(shaderResource);

    SAFE_RELEASE(texture);

    //ノーマルマップ用テクスチャーとそのレンダーターゲットビュー、シェーダーリソースビューの作成
    desc.format = Format::FORMAT_RGBA32_FLOAT;
    texture = renderer->texture2D()->createTexture2D(desc, nullptr);

    RTVDesc.Format = toFormat(desc.format);
    renderer->device()->CreateRenderTargetView(texture, &RTVDesc, &renderTarget);
    mRenderTargets.emplace_back(renderTarget);

    SRVDesc.Format = toFormat(desc.format);
    renderer->device()->CreateShaderResourceView(texture, &SRVDesc, &shaderResource);
    mShaderResources.emplace_back(shaderResource);

    SAFE_RELEASE(texture);

    //ポジションマップ用テクスチャーとそのレンダーターゲットビュー、シェーダーリソースビューの作成
    texture = renderer->texture2D()->createTexture2D(desc, nullptr);

    renderer->device()->CreateRenderTargetView(texture, &RTVDesc, &renderTarget);
    mRenderTargets.emplace_back(renderTarget);

    renderer->device()->CreateShaderResourceView(texture, &SRVDesc, &shaderResource);
    mShaderResources.emplace_back(shaderResource);

    SAFE_RELEASE(texture);

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
    bd.size = sizeof(MeshVertex) * 4;
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);
    SubResourceDesc sub;
    sub.data = vertices;

    mVertexBuffer = renderer->createBuffer(bd, &sub);
}

ID3D11RenderTargetView* GBuffer::getRenderTarget(unsigned index) const {
    return mRenderTargets[index];
}

ID3D11ShaderResourceView* GBuffer::getShaderResource(unsigned index) const {
    return mShaderResources[index];
}

ID3D11SamplerState* GBuffer::getSampler() const {
    return mSampler;
}

std::shared_ptr<Shader> GBuffer::shader() const {
    return mShader;
}

std::shared_ptr<Buffer> GBuffer::vertexBuffer() const {
    return mVertexBuffer;
}
