#include "Texture.h"
#include "BufferDesc.h"
#include "DirectX.h"
#include "Format.h"
#include "Game.h"
#include "IndexBuffer.h"
#include "Sampler.h"
#include "SamplerDesc.h"
#include "ShaderResourceView.h"
#include "SubResourceDesc.h"
#include "Usage.h"
#include "VertexBuffer.h"
#include "../DebugLayer/Debug.h"
#include "../Shader/Shader.h"
#include "../Utility/Directory.h"
#include "../Utility/FileUtil.h"

Texture::Texture(const std::string& fileName, bool isSprite) :
    mShaderResourceView(nullptr),
    mSampler(nullptr),
    mDesc() {
    if (!vertexBuffer || !indexBuffer) {
        //バーテックスバッファー作成
        createVertexBuffer();
        //インデックスバッファの作成
        createIndexBuffer();
    }
    //テクスチャー作成
    createTexture(fileName, isSprite);
    //テクスチャー用サンプラー作成
    createSampler();
}

Texture::~Texture() = default;

void Texture::end() {
    SAFE_DELETE(vertexBuffer);
    SAFE_DELETE(vertexBuffer3D);
    SAFE_DELETE(indexBuffer);
}

const TextureDesc& Texture::desc() const {
    return mDesc;
}

void Texture::setVSTextures(unsigned start, unsigned numTextures) const {
    mShaderResourceView->setVSShaderResources(start, numTextures);
}

void Texture::setPSTextures(unsigned start, unsigned numTextures) const {
    mShaderResourceView->setPSShaderResources(start, numTextures);
}

void Texture::setVSSamplers(unsigned start, unsigned numSamplers) const {
    mSampler->setVSSamplers(start, numSamplers);
}

void Texture::setPSSamplers(unsigned start, unsigned numSamplers) const {
    mSampler->setPSSamplers(start, numSamplers);
}

void Texture::createVertexBuffer() {
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

    vertexBuffer = new VertexBuffer(bd, &sub);

    static const TextureVertex vertices3D[] = {
        Vector3(-0.5f, 0.5f, 0.f), Vector2(0.f, 0.f), //左上
        Vector3(0.5f, 0.5f, 0.f), Vector2(1.f, 0.f), //右上
        Vector3(-0.5f, -0.5f, 0.f), Vector2(0.f, 1.f), //左下
        Vector3(0.5f, -0.5f, 0.f), Vector2(1.f, 1.f), //右下
    };

    sub.data = vertices3D;

    vertexBuffer3D = new VertexBuffer(bd, &sub);
}

void Texture::createIndexBuffer() {
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
    indexBuffer = new IndexBuffer(bd, &sub);
}

void Texture::createTexture(const std::string & filePath, bool isSprite) {
    if (isSprite) {
        Singleton<Directory>::instance().setTextureDirectory();
    } else {
        Singleton<Directory>::instance().setModelDirectory(filePath);
    }
    //ファイルからテクスチャ情報を取得
    D3DX11_IMAGE_INFO info;
    auto fileName = FileUtil::getFileNameFromDirectry(filePath);
    D3DX11GetImageInfoFromFileA(fileName.c_str(), nullptr, &info, nullptr);

    mDesc.width = info.Width;
    mDesc.height = info.Height;

    ID3D11ShaderResourceView* srv;

    if (FAILED(D3DX11CreateShaderResourceViewFromFileA(Singleton<DirectX>::instance().device(), fileName.c_str(), &toImageLoadInfo(mDesc), nullptr, &srv, nullptr))) {
        Debug::windowMessage(fileName + ": テクスチャ作成失敗");
    }

    mShaderResourceView = std::make_unique<ShaderResourceView>(srv);
}

void Texture::createSampler() {
    SamplerDesc sd;
    mSampler = std::make_unique<Sampler>(sd);
}

D3DX11_IMAGE_LOAD_INFO Texture::toImageLoadInfo(const TextureDesc & desc) const {
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
VertexBuffer* Texture::vertexBuffer3D = nullptr;
IndexBuffer* Texture::indexBuffer = nullptr;
