#include "Texture.h"
#include "../Device/Renderer.h"
#include "../Shader/Shader.h"
#include "../System/Buffer.h"
#include "../System/BufferDesc.h"
#include "../System/Game.h"
#include "../System/SubResourceDesc.h"
#include "../System/VertexStreamDesc.h"

Texture::Texture(std::shared_ptr<Renderer> renderer, const std::string& fileName, bool isSprite) :
    mDeviceContext(renderer->deviceContext()),
    mTexture(nullptr),
    mSampleLinear(nullptr) {
    if (!vertexBuffer || !indexBuffer) {
        //バーテックスバッファー作成
        createVertexBuffer(renderer);
        //インデックスバッファの作成
        createIndexBuffer(renderer);
    }
    //テクスチャー作成
    createTexture(renderer, fileName, isSprite);
    //テクスチャー用サンプラー作成
    createSampler(renderer);
}

Texture::~Texture() {
    SAFE_RELEASE(mTexture);
    SAFE_RELEASE(mSampleLinear);
}

void Texture::end() {
    SAFE_DELETE(vertexBuffer);
    SAFE_DELETE(indexBuffer);
}

const TextureDesc& Texture::desc() const {
    return mDesc;
}

void Texture::setVSTextures(unsigned start, unsigned numTextures) {
    mDeviceContext->VSSetShaderResources(start, numTextures, &mTexture);
}

void Texture::setPSTextures(unsigned start, unsigned numTextures) {
    mDeviceContext->PSSetShaderResources(start, numTextures, &mTexture);
}

void Texture::setVSSamplers(unsigned start, unsigned numSamplers) {
    mDeviceContext->VSSetSamplers(start, numSamplers, &mSampleLinear);
}

void Texture::setPSSamplers(unsigned start, unsigned numSamplers) {
    mDeviceContext->PSSetSamplers(start, numSamplers, &mSampleLinear);
}

void Texture::createVertexBuffer(std::shared_ptr<Renderer> renderer) {
    static const TextureVertex vertices[] = {
        Vector3(0.f, 0.f, 0.f), Vector2(0.f, 0.f), //左上
        Vector3(1.f, 0.f, 0.f), Vector2(1.f, 0.f), //右上
        Vector3(0.f, 1.f, 0.f), Vector2(0.f, 1.f), //左下
        Vector3(1.f, 1.f, 0.f), Vector2(1.f, 1.f), //右下
    };

    BufferDesc bd;
    bd.size = sizeof(TextureVertex) * 4;
    bd.usage = BufferUsage::BUFFER_USAGE_IMMUTABLE;
    bd.type = BufferType::BUFFER_TYPE_VERTEX;

    SubResourceDesc sub;
    sub.data = vertices;
    vertexBuffer = renderer->createRawBuffer(bd, &sub);
}

void Texture::createIndexBuffer(std::shared_ptr<Renderer> renderer) {
    static constexpr unsigned short indices[] = {
        0, 1, 2,
        1, 3, 2
    };
    BufferDesc bd;
    bd.size = sizeof(indices);
    bd.usage = BufferUsage::BUFFER_USAGE_IMMUTABLE;
    bd.type = BufferType::BUFFER_TYPE_INDEX;

    SubResourceDesc sub;
    sub.data = indices;
    indexBuffer = renderer->createRawBuffer(bd, &sub);
}

void Texture::createTexture(std::shared_ptr<Renderer> renderer, const std::string& fileName, bool isSprite) {
    if (isSprite) {
        setTextureDirectory();
    } else {
        setOBJDirectory();
    }
    //ファイルからテクスチャ情報を取得
    D3DX11_IMAGE_INFO info;
    D3DX11GetImageInfoFromFileA(fileName.c_str(), nullptr, &info, nullptr);

    mDesc.width = info.Width;
    mDesc.height = info.Height;

    if (FAILED(D3DX11CreateShaderResourceViewFromFileA(renderer->device(), fileName.c_str(), &toImageLoadInfo(mDesc), nullptr, &mTexture, nullptr))) {
        MSG(L"テクスチャ作成失敗");
    }
}

void Texture::createSampler(std::shared_ptr<Renderer> renderer) {
    D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    renderer->device()->CreateSamplerState(&sd, &mSampleLinear);
}

D3DX11_IMAGE_LOAD_INFO Texture::toImageLoadInfo(const TextureDesc& desc) const {
    D3DX11_IMAGE_LOAD_INFO info;
    info.Width = desc.width;
    info.Height = desc.height;
    info.Depth = desc.depth;
    info.FirstMipLevel = desc.firstMipLevel;
    info.MipLevels = desc.mipLevels;
    info.Usage = toUsage(desc.usage);
    info.BindFlags = toBind(desc.bindFlags);
    info.CpuAccessFlags = toCPUAccess(desc.cpuAccessFlags);
    info.MiscFlags = desc.miscFlags;
    info.Format = toFormat(desc.format);
    info.Filter = toFilter(desc.filter);
    info.MipFilter = toFilter(desc.mipFilter);
    info.pSrcInfo = desc.srcInfo;

    return info;
}

D3D11_USAGE Texture::toUsage(TextureUsage usage) const {
    const D3D11_USAGE usages[] = {
        D3D11_USAGE_DEFAULT,
        D3D11_USAGE_IMMUTABLE,
        D3D11_USAGE_DYNAMIC,
        D3D11_USAGE_STAGING
    };
    return usages[static_cast<unsigned>(usage)];
}

unsigned Texture::toBind(TextureBind bind) const {
    const unsigned binds[] = {
        D3D11_BIND_SHADER_RESOURCE,
        D3D11_BIND_RENDER_TARGET,
    };
    return binds[static_cast<unsigned>(bind)];
}

unsigned Texture::toCPUAccess(TextureCPUAccessFlag flag) const {
    const unsigned accesses[] = {
        0,
        D3D11_CPU_ACCESS_WRITE,
        D3D11_CPU_ACCESS_READ
    };
    return accesses[static_cast<unsigned>(flag)];
}

DXGI_FORMAT Texture::toFormat(TextureFormat format) const {
    const DXGI_FORMAT formats[] = {
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_R8G8B8A8_UNORM
    };
    return formats[static_cast<unsigned>(format)];
}

unsigned Texture::toFilter(TextureFilter filter) const {
    const unsigned filters[] = {
        D3DX11_FILTER_POINT,
        D3DX11_FILTER_LINEAR,
        D3DX11_FILTER_TRIANGLE,
    };
    return filters[static_cast<unsigned>(filter)];
}

Buffer* Texture::vertexBuffer = nullptr;
Buffer* Texture::indexBuffer = nullptr;
