#include "Texture.h"
#include "Sprite.h"
#include "../Device/Renderer.h"
#include "../Shader/Shader.h"
#include "../System/Buffer.h"
#include "../System/BufferDesc.h"
#include "../System/Game.h"
#include "../System/InputElement.h"
#include "../System/SubResourceDesc.h"
#include "../System/VertexStreamDesc.h"

Texture::Texture(std::shared_ptr<Renderer> renderer, const char* fileName) :
    mRenderer(renderer),
    mTexture(nullptr),
    mSampleLinear(nullptr) {
    if (!mVertexBuffer || !mIndexBuffer) {
        //バーテックスバッファー作成
        createVertexBuffer();
        //インデックスバッファの作成
        createIndexBuffer();
    }
    //テクスチャー作成
    createTexture(fileName);
    //テクスチャー用サンプラー作成
    createSampler();
}

Texture::~Texture() {
    SAFE_RELEASE(mTexture);
    SAFE_RELEASE(mSampleLinear);
}

void Texture::end() {
    SAFE_DELETE(mVertexBuffer);
    SAFE_DELETE(mIndexBuffer);
}

void Texture::drawAll(std::vector<std::shared_ptr<Sprite>> sprites, std::shared_ptr<Renderer> renderer) {
    if (sprites.empty()) {
        return;
    }
    //スプライト共通作業
    //プロジェクション
    Matrix4 proj = Matrix4::identity;
    //原点をスクリーン左上にするために平行移動
    proj.m[3][0] = -1.f;
    proj.m[3][1] = 1.f;
    //ピクセル単位で扱うために
    proj.m[0][0] = 2.f / Game::WINDOW_WIDTH;
    proj.m[1][1] = -2.f / Game::WINDOW_HEIGHT;

    //プリミティブ・トポロジーをセット
    renderer->setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_STRIP);
    //バーテックスバッファーをセット
    VertexStreamDesc stream;
    stream.buffer = mVertexBuffer;
    stream.offset = 0;
    stream.stride = sizeof(TextureVertex);
    renderer->setVertexBuffer(&stream);
    //インデックスバッファーをセット
    renderer->setIndexBuffer(mIndexBuffer);

    for (auto itr = sprites.begin(); itr != sprites.end(); ++itr) {
        (*itr)->draw(proj);
    }
}

ID3D11ShaderResourceView* Texture::texture() const {
    return mTexture;
}

const TextureDesc& Texture::desc() const {
    return mDesc;
}

ID3D11SamplerState* Texture::getSampler() const {
    return mSampleLinear;
}

void Texture::setVSTextures(unsigned start, unsigned numTextures) {
    mRenderer->deviceContext()->VSSetShaderResources(start, numTextures, &mTexture);
}

void Texture::setPSTextures(unsigned start, unsigned numTextures) {
    mRenderer->deviceContext()->PSSetShaderResources(start, numTextures, &mTexture);
}

void Texture::createVertexBuffer() {
    TextureVertex vertices[] = {
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
    mVertexBuffer = mRenderer->createRawBuffer(bd, &sub);
}

void Texture::createIndexBuffer() {
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
    mIndexBuffer = mRenderer->createRawBuffer(bd, &sub);
}

void Texture::createTexture(const char* fileName) {
    setTextureDirectory();
    //ファイルからテクスチャ情報を取得
    D3DX11_IMAGE_INFO info;
    D3DX11GetImageInfoFromFileA(fileName, nullptr, &info, nullptr);

    mDesc.width = info.Width;
    mDesc.height = info.Height;

    if (FAILED(D3DX11CreateShaderResourceViewFromFileA(mRenderer->device(), fileName, &toImageLoadInfo(mDesc), nullptr, &mTexture, nullptr))) {
        MSG(L"テクスチャ作成失敗");
    }
}

void Texture::createSampler() {
    D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    mRenderer->device()->CreateSamplerState(&sd, &mSampleLinear);
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
        DXGI_FORMAT_R8G8B8A8_UNORM,
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

Buffer* Texture::mVertexBuffer = nullptr;
Buffer* Texture::mIndexBuffer = nullptr;
