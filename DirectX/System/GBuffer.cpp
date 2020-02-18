#include "GBuffer.h"
#include "Buffer.h"
#include "BufferDesc.h"
#include "Game.h"
#include "IndexBuffer.h"
#include "RenderTargetView.h"
#include "RenderTargetViewDesc.h"
#include "Sampler.h"
#include "SamplerDesc.h"
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
    mVertexBuffer(nullptr),
    mIndexBuffer(nullptr) {
}

GBuffer::~GBuffer() = default;

void GBuffer::create(std::shared_ptr<Renderer> renderer) {
    //Deferred 関連 なおそれぞれ深度ステンシルを作るわけではない。サイズが同じなので通常のものを使い回せる。
    Texture2DDesc desc;
    ShaderResourceViewDesc srvDesc;
    RenderTargetViewDesc rtvDesc;

    //カラー
    desc.width = Game::WINDOW_WIDTH;
    desc.height = Game::WINDOW_HEIGHT;
    desc.format = Format::FORMAT_RGBA32_FLOAT;
    //desc.format = Format::FORMAT_RGBA8_UNORM;
    desc.usage = Usage::USAGE_DEFAULT;
    desc.bindFlags =
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_RENDER_TARGET) |
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_SHADER_RESOURCE);
    auto texture = renderer->createTexture2D(desc);

    rtvDesc.format = desc.format;
    mRenderTargets.emplace_back(std::make_shared<RenderTargetView>(renderer, texture, &rtvDesc));

    srvDesc.format = desc.format;
    mShaderResourceViews.emplace_back(std::make_shared<ShaderResourceView>(renderer, texture, &srvDesc));

    //ノーマル
    desc.format = Format::FORMAT_RGBA32_FLOAT;
    auto texture2 = renderer->createTexture2D(desc);

    rtvDesc.format = desc.format;
    mRenderTargets.emplace_back(std::make_shared<RenderTargetView>(renderer, texture2, &rtvDesc));

    srvDesc.format = desc.format;
    mShaderResourceViews.emplace_back(std::make_shared<ShaderResourceView>(renderer, texture2, &srvDesc));

    //ポジション
    desc.format = Format::FORMAT_RGBA32_FLOAT;
    auto texture3 = renderer->createTexture2D(desc);

    mRenderTargets.emplace_back(std::make_shared<RenderTargetView>(renderer, texture3, &rtvDesc));

    mShaderResourceViews.emplace_back(std::make_shared<ShaderResourceView>(renderer, texture3, &srvDesc));

    //サンプラー作成
    SamplerDesc sd;
    mSampler = renderer->createSamplerState(sd);

    //各種生成
    createShader(renderer);
    createVertexBuffer(renderer);
    createIndexBuffer(renderer);
}

std::shared_ptr<RenderTargetView> GBuffer::getRenderTarget(unsigned index) const {
    return mRenderTargets[index];
}

std::shared_ptr<ShaderResourceView> GBuffer::getShaderResourceView(unsigned index) const {
    return mShaderResourceViews[index];
}

std::shared_ptr<Sampler> GBuffer::getSampler() const {
    return mSampler;
}

std::shared_ptr<Shader> GBuffer::shader() const {
    return mShader;
}

std::shared_ptr<VertexBuffer> GBuffer::getVertexBuffer() const {
    return mVertexBuffer;
}

std::shared_ptr<IndexBuffer> GBuffer::getIndexBuffer() const {
    return mIndexBuffer;
}

void GBuffer::createShader(std::shared_ptr<Renderer> renderer) {
    //シェーダー生成
    mShader = renderer->createShader("Deferred.hlsl");
    mShader->createConstantBuffer(sizeof(GBufferShaderConstantBuffer));
}

void GBuffer::createVertexBuffer(std::shared_ptr<Renderer> renderer) {
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

void GBuffer::createIndexBuffer(std::shared_ptr<Renderer> renderer) {
    //インデックスバッファ作成
    static constexpr unsigned short indices[] = {
        0, 1, 2,
        1, 3, 2
    };
    BufferDesc bd;
    bd.size = sizeof(indices);
    bd.usage = Usage::USAGE_IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_INDEX);

    SubResourceDesc sub;
    sub.data = indices;
    mIndexBuffer = renderer->createIndexBuffer(bd, &sub);
}
