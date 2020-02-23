#pragma once

#include "DirectXIncLib.h"
#include "Game.h"
#include "TextureDesc.h"
#include "../Utility/Math.h"
#include <memory>
#include <string>

//頂点の構造体
struct TextureVertex {
    Vector3 mPos; //位置
    Vector2 mTex; //テクスチャ座標
};

struct TextureShaderConstantBuffer {
    ALIGN16 Matrix4 mWorld;
    ALIGN16 Matrix4 mProjection;
    ALIGN16 Vector4 mColor;
    ALIGN16 Vector4 mUV;
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
    void setVSTextures(unsigned start = 0, unsigned numTextures = 1);
    void setPSTextures(unsigned start = 0, unsigned numTextures = 1);
    //サンプラーの登録
    void setVSSamplers(unsigned start = 0, unsigned numSamplers = 1);
    void setPSSamplers(unsigned start = 0, unsigned numSamplers = 1);

private:
    void createVertexBuffer();
    void createIndexBuffer();
    void createTexture(const std::string& fileName, bool isSprite);
    void createSampler();
    //各種変換
    D3DX11_IMAGE_LOAD_INFO toImageLoadInfo(const TextureDesc& desc) const;
    unsigned toFilter(TextureFilter filter) const;

public:
    static VertexBuffer* vertexBuffer;
    static IndexBuffer* indexBuffer;

private:
    std::unique_ptr<ShaderResourceView> mShaderResourceView;
    std::unique_ptr<Sampler> mSampler;
    TextureDesc mDesc;
};
