#pragma once

#include "SystemInclude.h"
#include "../DirectX/TextureDesc.h"
#include "../Math/Math.h"
#include <memory>
#include <string>

//頂点の構造体
struct TextureVertex {
    Vector3 mPos; //位置
    Vector2 mTex; //テクスチャ座標
};

struct TextureConstantBuffer {
    Matrix4 wp;
    Vector4 color;
    Vector4 uv;
};

class IndexBuffer;
class Sampler;
class ShaderResourceView;
class VertexBuffer;

class Texture {
public:
    Texture(const std::string& fileName, bool isSprite);
    ~Texture();
    static void end();
    const TextureDesc& desc() const;
    //テクスチャの登録
    void setVSTextures(unsigned start = 0, unsigned numTextures = 1) const;
    void setPSTextures(unsigned start = 0, unsigned numTextures = 1) const;
    //サンプラーの登録
    void setVSSamplers(unsigned start = 0, unsigned numSamplers = 1) const;
    void setPSSamplers(unsigned start = 0, unsigned numSamplers = 1) const;

private:
    void createVertexBuffer();
    void createIndexBuffer();
    void createTexture(const std::string& filePath, bool isSprite);
    void createSampler();
    //各種変換
    D3DX11_IMAGE_LOAD_INFO toImageLoadInfo(const TextureDesc& desc) const;
    unsigned toFilter(TextureFilter filter) const;

public:
    static VertexBuffer* vertexBuffer;
    static VertexBuffer* vertexBuffer3D;
    static IndexBuffer* indexBuffer;

private:
    std::unique_ptr<ShaderResourceView> mShaderResourceView;
    std::unique_ptr<Sampler> mSampler;
    TextureDesc mDesc;
};
