#pragma once

#include "DirectXIncLib.h"
#include "Game.h"
#include "../Utility/Math.h"
#include <memory>
#include <vector>

class Buffer;
class Renderer;
class Shader;

struct GBufferShaderConstantBuffer {
    ALIGN16 Vector3 lightDir; //ライト方向
    ALIGN16 Vector3 cameraPos; //カメラ位置
    ALIGN16 Vector3 ambientLight; //環境光
};

class GBuffer {
public:
    enum class Type {
        DIFFUSE,
        NORMAL,
        WORLD_POS,
        NUM_GBUFFER_TEXTURES
    };

    GBuffer();
    ~GBuffer();

    //Gバッファの生成
    void create(std::shared_ptr<Renderer> renderer);

    ID3D11Texture2D* getTexture(unsigned index) const;
    ID3D11RenderTargetView* getRenderTarget(unsigned index) const;
    ID3D11ShaderResourceView* getShaderResource(unsigned index) const;
    std::shared_ptr<Shader> shader() const;
    std::shared_ptr<Buffer> vertexBuffer() const;

private:
    //Gバッファに割り当てられたテクスチャ
    std::vector<ID3D11Texture2D*> mTextures;
    std::vector<ID3D11RenderTargetView*> mRenderTargets;
    std::vector<ID3D11ShaderResourceView*> mShaderResources;
    std::shared_ptr<Shader> mShader;
    std::shared_ptr<Buffer> mVertexBuffer;
};
