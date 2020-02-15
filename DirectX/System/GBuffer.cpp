#include "GBuffer.h"
#include "Buffer.h"
#include "BufferDesc.h"
#include "Game.h"
#include "SubResourceDesc.h"
#include "../Device/Renderer.h"
#include "../Mesh/MeshLoader.h"
#include "../Shader/Shader.h"

GBuffer::GBuffer() :
    mShader(nullptr),
    mVertexBuffer(nullptr) {
}

GBuffer::~GBuffer() {
    for (auto&& t : mTextures) {
        SAFE_RELEASE(t);
    }
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
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));
    ZeroMemory(&RTVDesc, sizeof(RTVDesc));

    //カラーマップ用テクスチャーとそのレンダーターゲットビュー、シェーダーリソースビューの作成
    desc.Width = Game::WINDOW_WIDTH;
    desc.Height = Game::WINDOW_HEIGHT;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    ID3D11Texture2D* texture;
    renderer->device()->CreateTexture2D(&desc, NULL, &texture);
    mTextures.emplace_back(texture);

    RTVDesc.Format = desc.Format;
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.MipSlice = 0;
    ID3D11RenderTargetView* renderTarget;
    renderer->device()->CreateRenderTargetView(texture, &RTVDesc, &renderTarget);
    mRenderTargets.emplace_back(renderTarget);

    SRVDesc.Format = desc.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;
    ID3D11ShaderResourceView* shaderResource;
    renderer->device()->CreateShaderResourceView(texture, &SRVDesc, &shaderResource);
    mShaderResources.emplace_back(shaderResource);

    //ノーマルマップ用テクスチャーとそのレンダーターゲットビュー、シェーダーリソースビューの作成
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    renderer->device()->CreateTexture2D(&desc, NULL, &texture);
    mTextures.emplace_back(texture);

    RTVDesc.Format = desc.Format;
    renderer->device()->CreateRenderTargetView(texture, &RTVDesc, &renderTarget);
    mRenderTargets.emplace_back(renderTarget);

    SRVDesc.Format = desc.Format;
    renderer->device()->CreateShaderResourceView(texture, &SRVDesc, &shaderResource);
    mShaderResources.emplace_back(shaderResource);

    //ポジションマップ用テクスチャーとそのレンダーターゲットビュー、シェーダーリソースビューの作成
    renderer->device()->CreateTexture2D(&desc, NULL, &texture);
    mTextures.emplace_back(texture);

    renderer->device()->CreateRenderTargetView(texture, &RTVDesc, &renderTarget);
    mRenderTargets.emplace_back(renderTarget);

    renderer->device()->CreateShaderResourceView(texture, &SRVDesc, &shaderResource);
    mShaderResources.emplace_back(shaderResource);

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
    bd.usage = BufferUsage::BUFFER_USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);
    SubResourceDesc sub;
    sub.data = vertices;

    //mVertexBuffer = renderer->createBuffer(bd, &sub);
    mVertexBuffer = std::make_shared<Buffer>(renderer->device(), bd, &sub);
}

ID3D11Texture2D* GBuffer::getTexture(unsigned index) const {
    return mTextures[index];
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
