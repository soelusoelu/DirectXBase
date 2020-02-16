#include "Texture.h"
#include "../Device/Renderer.h"
#include "../Shader/Shader.h"
#include "../System/Buffer.h"
#include "../System/BufferDesc.h"
#include "../System/Format.h"
#include "../System/Game.h"
#include "../System/ShaderResourceView.h"
#include "../System/SubResourceDesc.h"
#include "../System/Usage.h"
#include "../System/VertexBuffer.h"

Texture::Texture(std::shared_ptr<Renderer> renderer, const std::string& fileName, bool isSprite) :
    mDeviceContext(renderer->deviceContext()),
    mShaderResourceView(nullptr),
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
    mShaderResourceView->setVSShaderResources(start, numTextures);
}

void Texture::setPSTextures(unsigned start, unsigned numTextures) {
    mShaderResourceView->setPSShaderResources(start, numTextures);
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
    bd.oneSize = sizeof(TextureVertex);
    bd.size = bd.oneSize * 4; //頂点が4つだから
    bd.usage = Usage::USAGE_IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);

    SubResourceDesc sub;
    sub.data = vertices;

    vertexBuffer = new VertexBuffer(renderer, bd, &sub);
}

void Texture::createIndexBuffer(std::shared_ptr<Renderer> renderer) {
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

    ID3D11ShaderResourceView* srv;

    if (FAILED(D3DX11CreateShaderResourceViewFromFileA(renderer->device(), fileName.c_str(), &toImageLoadInfo(mDesc), nullptr, &srv, nullptr))) {
        MSG(L"テクスチャ作成失敗");
    }

    mShaderResourceView = std::make_shared<ShaderResourceView>(renderer, srv);
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
    info.BindFlags = desc.bindFlags;
    info.CpuAccessFlags = desc.cpuAccessFlags;
    info.MiscFlags = desc.miscFlags;
    info.Format = toFormat(desc.format);
    info.Filter = toFilter(desc.filter);
    info.MipFilter = toFilter(desc.mipFilter);
    info.pSrcInfo = desc.srcInfo;

    return info;
}

unsigned Texture::toFilter(TextureFilter filter) const {
    const unsigned filters[] = {
        D3DX11_FILTER_POINT,
        D3DX11_FILTER_LINEAR,
        D3DX11_FILTER_TRIANGLE,
    };
    return filters[static_cast<unsigned>(filter)];
}

VertexBuffer* Texture::vertexBuffer = nullptr;
Buffer* Texture::indexBuffer = nullptr;
