#pragma once

#include "DirectXIncLib.h"
#include "Game.h"
#include "../Utility/Math.h"
#include <memory>
#include <vector>

class IndexBuffer;
class Renderer;
class RenderTargetView;
class Sampler;
class Shader;
class ShaderResourceView;
class VertexBuffer;

struct GBufferShaderConstantBuffer {
    ALIGN16 Vector3 lightDir; //ライト方向
    ALIGN16 Vector3 cameraPos; //カメラ位置
    ALIGN16 Vector3 ambientLight; //環境光
};

class GBuffer {
    using SRVPtr = std::shared_ptr<ShaderResourceView>;
    using SRVPtrArray = std::vector<SRVPtr>;
    using RTVPtr = std::shared_ptr<RenderTargetView>;
    using RTVPtrArray = std::vector<RTVPtr>;
public:
    enum class Type {
        DIFFUSE,
        NORMAL,
        WORLD_POS,
        SPECULAR,
        NUM_GBUFFER_TEXTURES
    };

    GBuffer();
    ~GBuffer();

    //Gバッファの生成
    void create(std::shared_ptr<Renderer> renderer);

    RTVPtr getRenderTarget(unsigned index) const;
    SRVPtr getShaderResourceView(unsigned index) const;
    std::shared_ptr<Sampler> getSampler() const;
    std::shared_ptr<Shader> shader() const;
    std::shared_ptr<VertexBuffer> getVertexBuffer() const;
    std::shared_ptr<IndexBuffer> getIndexBuffer() const;

private:
    void createShader(std::shared_ptr<Renderer> renderer);
    void createVertexBuffer(std::shared_ptr<Renderer> renderer);
    void createIndexBuffer(std::shared_ptr<Renderer> renderer);

private:
    //Gバッファに割り当てられたテクスチャ
    RTVPtrArray mRenderTargets;
    SRVPtrArray mShaderResourceViews;
    std::shared_ptr<Sampler> mSampler;
    std::shared_ptr<Shader> mShader;
    std::shared_ptr<VertexBuffer> mVertexBuffer;
    std::shared_ptr<IndexBuffer> mIndexBuffer;
};
