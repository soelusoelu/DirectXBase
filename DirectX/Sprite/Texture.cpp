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
    mVertexBuffer(nullptr),
    mVertexLayout(nullptr),
    mTexture(nullptr),
    mSampleLinear(nullptr) {
    if (!mIndexBuffer) {
        //インデックスバッファの作成
        createIndexBuffer(renderer);
    }
    //テクスチャー作成
    createTexture(renderer, fileName);
    //テクスチャー用サンプラー作成
    createSampler(renderer);
    //バーテックスバッファー作成
    createVertexBuffer(renderer);
}

Texture::~Texture() {
    SAFE_RELEASE(mTexture);
    SAFE_RELEASE(mSampleLinear);
}

void Texture::end() {
    SAFE_DELETE(mIndexBuffer);
}

void Texture::createInputLayout(std::shared_ptr<Renderer> renderer, ID3D10Blob* compiledShader) {
    constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    mVertexLayout = renderer->createInputLayout(layout, numElements, compiledShader);
}

void Texture::createVertexBuffer(std::shared_ptr<Renderer> renderer) {
    auto w = mDesc.width;
    auto h = mDesc.height;
    TextureVertex vertices[] = {
        Vector3(0.f, 0.f, 0.f), Vector2(0.f, 0.f), //左上
        Vector3(w, 0.f, 0.f), Vector2(1.f, 0.f), //右上
        Vector3(0.f, h, 0.f), Vector2(0.f, 1.f), //左下
        Vector3(w, h, 0.f), Vector2(1.f, 1.f), //右下
    };

    BufferDesc bd;
    bd.size = sizeof(TextureVertex) * 4;
    bd.usage = BufferUsage::BUFFER_USAGE_IMMUTABLE;
    bd.type = BufferType::BUFFER_TYPE_VERTEX;

    SubResourceDesc sub;
    sub.data = vertices;
    mVertexBuffer = renderer->createBuffer(bd, &sub);
}

void Texture::createVertexBuffer(std::shared_ptr<Renderer> renderer, const Vector2INT& size) {
    TextureVertex vertices[] = {
        Vector3(0.f, 0.f, 0.f), Vector2(0.f, 0.f), //左上
        Vector3(size.x, 0.f, 0.f), Vector2(1.f, 0.f), //右上
        Vector3(0.f, size.y, 0.f), Vector2(0.f, 1.f), //左下
        Vector3(size.x, size.y, 0.f), Vector2(1.f, 1.f), //右下
    };

    BufferDesc bd;
    bd.size = sizeof(TextureVertex) * 4;
    bd.usage = BufferUsage::BUFFER_USAGE_IMMUTABLE;
    bd.type = BufferType::BUFFER_TYPE_VERTEX;

    SubResourceDesc sub;
    sub.data = vertices;
    mVertexBuffer = renderer->createBuffer(bd, &sub);
}

void Texture::drawAll(std::list<std::shared_ptr<Sprite>> sprites, std::shared_ptr<Renderer> renderer) {
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

std::shared_ptr<Buffer> Texture::getVertexBuffer() const {
    return mVertexBuffer;
}

std::shared_ptr<InputElement> Texture::getVertexlayout() const {
    return mVertexLayout;
}

ID3D11SamplerState* Texture::getSampler() const {
    return mSampleLinear;
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
    mIndexBuffer = renderer->createRawBuffer(bd, &sub);
}

void Texture::createTexture(std::shared_ptr<Renderer> renderer, const char* fileName) {
    setTextureDirectory();
    //ファイルからテクスチャ情報を取得
    D3DX11_IMAGE_INFO info;
    D3DX11GetImageInfoFromFileA(fileName, nullptr, &info, nullptr);

    mDesc.width = info.Width;
    mDesc.height = info.Height;

    if (FAILED(D3DX11CreateShaderResourceViewFromFileA(renderer->device(), fileName, &toImageLoadInfo(mDesc), nullptr, &mTexture, nullptr))) {
        MessageBox(0, L"テクスチャ作成失敗", NULL, MB_OK);
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

Buffer* Texture::mIndexBuffer = nullptr;
