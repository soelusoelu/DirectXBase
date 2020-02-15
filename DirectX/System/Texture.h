#pragma once

#include "../System/DirectXIncLib.h"
#include "../System/Game.h"
#include "../System/TextureDesc.h"
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

class Buffer;
class InputElement;
class Sprite;
class Renderer;

class Texture {
public:
    Texture(std::shared_ptr<Renderer> renderer, const std::string& fileName, bool isSprite);
    ~Texture();
    static void end();
    const TextureDesc& desc() const;
    //テクスチャの登録
    void setVSTextures(unsigned start = 0, unsigned numTextures = 1);
    void setPSTextures(unsigned start = 0, unsigned numTextures = 1);
    //サンプラーの登録
    void setVSSamplers(unsigned start = 0, unsigned numSamplers = 1);
    void setPSSamplers(unsigned start = 0, unsigned numSamplers = 1);

public:
    static Buffer* vertexBuffer;
    static Buffer* indexBuffer;

private:
    void createVertexBuffer(std::shared_ptr<Renderer> renderer);
    void createIndexBuffer(std::shared_ptr<Renderer> renderer);
    void createTexture(std::shared_ptr<Renderer> renderer, const std::string& fileName, bool isSprite);
    void createSampler(std::shared_ptr<Renderer> renderer);
    //各種変換
    D3DX11_IMAGE_LOAD_INFO toImageLoadInfo(const TextureDesc& desc) const;
    D3D11_USAGE toUsage(TextureUsage usage) const;
    DXGI_FORMAT toFormat(TextureFormat format) const;
    unsigned toFilter(TextureFilter filter) const;

private:
    ID3D11DeviceContext* mDeviceContext;
    ID3D11ShaderResourceView* mTexture;
    ID3D11SamplerState* mSampleLinear;
    TextureDesc mDesc;
};
