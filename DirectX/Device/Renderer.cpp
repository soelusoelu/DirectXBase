#include "Renderer.h"
#include "Sound.h"
#include "../Mesh/MeshLoader.h"
#include "../Shader/Shader.h"
#include "../System/Buffer.h"
#include "../System/DirectXIncLib.h"
#include "../System/GBuffer.h"
#include "../System/InputElement.h"
#include "../System/SubResourceDesc.h"
#include "../System/VertexStreamDesc.h"
#include "../System/ViewportDesc.h"
#include "../Sprite/Texture.h"
#include "../Sprite/Sprite.h"
#include <algorithm>

Renderer::Renderer(const HWND& hWnd) :
    mDevice(nullptr),
    mDeviceContext(nullptr),
    mSwapChain(nullptr),
    mRenderTargetView(nullptr),
    mDepthStencil(nullptr),
    mDepthStencilView(nullptr),
    mRasterizerState(nullptr),
    mRasterizerStateBack(nullptr),
    mDepthStencilState(nullptr),
    mBlendState(nullptr),
    mSoundBase(std::make_unique<SoundBase>()),
    mGBuffer(std::make_shared<GBuffer>()) {
    createDeviceAndSwapChain(hWnd);
    createRenderTargetView();
    createDepthStencilView();
    setDefaultRenderTarget();
    createDepthStencilState();
    ViewportDesc desc;
    desc.width = Game::WINDOW_WIDTH;
    desc.height = Game::WINDOW_HEIGHT;
    setViewport(desc);
    createRasterizerState();
    createBlendState();
}

Renderer::~Renderer() {
    mShaders.clear();
    mTextures.clear();
    mSounds.clear();
    mMeshLoaders.clear();

    SAFE_RELEASE(mBlendState);
    SAFE_RELEASE(mDepthStencilState);
    SAFE_RELEASE(mRasterizerStateBack);
    SAFE_RELEASE(mRasterizerState);
    SAFE_RELEASE(mDepthStencilView);
    SAFE_RELEASE(mDepthStencil);
    SAFE_RELEASE(mRenderTargetView);
    SAFE_RELEASE(mSwapChain);
    SAFE_RELEASE(mDeviceContext);
    SAFE_RELEASE(mDevice);
}

void Renderer::initialize() {
    mGBuffer->create(shared_from_this());
}

ID3D11Device* Renderer::device() const {
    return mDevice;
}

ID3D11DeviceContext* Renderer::deviceContext() const {
    return mDeviceContext;
}

std::shared_ptr<GBuffer> Renderer::getGBuffer() const {
    return mGBuffer;
}

Buffer* Renderer::createRawBuffer(const BufferDesc& desc, const SubResourceDesc* data) const {
    return new Buffer(mDevice, desc, data);
}

std::shared_ptr<Buffer> Renderer::createBuffer(const BufferDesc& desc, const SubResourceDesc* data) const {
    return std::make_shared<Buffer>(mDevice, desc, data);
}

void Renderer::setViewport(const ViewportDesc& desc) {
    //ビューポートの設定
    D3D11_VIEWPORT vp;
    vp.Width = desc.width;
    vp.Height = desc.height;
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    mDeviceContext->RSSetViewports(1, &vp);
}

void Renderer::setVertexBuffer(const VertexStreamDesc* stream, unsigned numStream, unsigned start) {
    /* IASetVertexBuffers
        使い始めのスロット番号
        頂点バッファ配列の要素数
        頂点バッファ配列の先頭ポインタ
        INPUT_ELEMENT_DESC構造体のサイズが入った配列への先頭ポインタ(stride(読み込み単位)として扱うため)
        頂点バッファ配列の各頂点バッファの頭出しをするオフセット値の配列
    */
    ID3D11Buffer* buffer;
    if (stream->buffer) {
        buffer = stream->buffer->buffer();
    } else {
        buffer = stream->sharedBuffer->buffer();
    }
    mDeviceContext->IASetVertexBuffers(start, numStream, &buffer, &stream->stride, &stream->offset);
}

void Renderer::setIndexBuffer(Buffer* buffer, unsigned offset) {
    mDeviceContext->IASetIndexBuffer(buffer->buffer(), DXGI_FORMAT_R16_UINT, offset);
}

void Renderer::setIndexBuffer(std::shared_ptr<Buffer> buffer, unsigned offset) {
    mDeviceContext->IASetIndexBuffer(buffer->buffer(), DXGI_FORMAT_R32_UINT, offset);
}

void Renderer::setPrimitive(PrimitiveType primitive) {
    mDeviceContext->IASetPrimitiveTopology(toPrimitiveMode(primitive));
}

void Renderer::setRenderTargets(ID3D11RenderTargetView* targets[], unsigned numTargets) {
    mDeviceContext->OMSetRenderTargets(numTargets, targets, mDepthStencilView);
}

void Renderer::setDefaultRenderTarget() {
    mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void Renderer::setRasterizerStateFront() {
    mDeviceContext->RSSetState(mRasterizerState);
}

void Renderer::setRasterizerStateBack() {
    mDeviceContext->RSSetState(mRasterizerStateBack);
}

std::shared_ptr<Shader> Renderer::createShader(const std::string& fileName) {
    std::shared_ptr<Shader> shader;
    auto itr = mShaders.find(fileName);
    if (itr != mShaders.end()) { //既に読み込まれている
        shader = itr->second;
    } else { //初読み込み
        shader = std::make_shared<Shader>(shared_from_this(), fileName);
        mShaders.emplace(fileName, shader);
    }
    return shader;
}

std::shared_ptr<Texture> Renderer::createTexture(const std::string& fileName, bool isSprite) {
    std::shared_ptr<Texture> texture;
    auto itr = mTextures.find(fileName);
    if (itr != mTextures.end()) { //既に読み込まれている
        texture = itr->second;
    } else { //初読み込み
        texture = std::make_shared<Texture>(shared_from_this(), fileName, isSprite);
        mTextures.emplace(fileName, texture);
    }
    return texture;
}

std::shared_ptr<Sound> Renderer::createSound(const std::string& fileName) {
    std::shared_ptr<Sound> sound;
    auto itr = mSounds.find(fileName);
    if (itr != mSounds.end()) { //既に読み込まれている
        sound = itr->second;
    } else { //初読み込み
        sound = std::make_shared<Sound>();
        mSoundBase->load(fileName, &sound);
        mSounds.emplace(fileName, sound);
    }
    return sound;
}

std::shared_ptr<Sound> Renderer::createSE(const std::string& fileName) {
    auto sound = createSound(fileName);
    mSoundBase->createSourceVoice(&sound);
    return sound;
}

std::shared_ptr<MeshLoader> Renderer::createMesh(const std::string& fileName) {
    std::shared_ptr<MeshLoader> mesh;
    auto itr = mMeshLoaders.find(fileName);
    if (itr != mMeshLoaders.end()) { //既に読み込まれている
        mesh = itr->second;
    } else { //初読み込み
        mesh = std::make_shared<MeshLoader>(shared_from_this(), fileName);
        mMeshLoaders.emplace(fileName, mesh);
    }
    return mesh;
}

void Renderer::addPointLight(PointLightComponent* light) {
    mPointLigths.emplace_back(light);
}

void Renderer::removePointLight(PointLightComponent* light) {
    auto itr = std::find(mPointLigths.begin(), mPointLigths.end(), light);
    mPointLigths.erase(itr);
}

void Renderer::draw(unsigned numVertex, unsigned start) {
    mDeviceContext->Draw(numVertex, start);
}

void Renderer::drawIndexed(unsigned numIndices, unsigned startIndex, int startVertex) {
    mDeviceContext->DrawIndexed(numIndices, startIndex, startVertex);
}

void Renderer::clear(float r, float g, float b, float a, bool depth, bool stencil) {
    clearRenderTarget(mRenderTargetView, r, g, b, a);
    clearDepthStencilView(depth, stencil);
}

void Renderer::clearRenderTarget(ID3D11RenderTargetView* target, float r, float g, float b, float a) {
    const float clearColor[4] = { r, g, b, a }; //クリア色作成 RGBAの順
    mDeviceContext->ClearRenderTargetView(target, clearColor); //画面クリア
}

void Renderer::clearDepthStencilView(bool depth, bool stencil) {
    unsigned mask = 0;
    if (depth) {
        mask |= D3D11_CLEAR_DEPTH;
    }
    if (stencil) {
        mask |= D3D11_CLEAR_STENCIL;
    }
    mDeviceContext->ClearDepthStencilView(mDepthStencilView, mask, 1.f, 0); //深度バッファクリア
}

void Renderer::present() {
    mSwapChain->Present(0, 0); //画面更新(バックバッファをフロントバッファに)
}

void Renderer::createDeviceAndSwapChain(const HWND& hWnd) {
    // デバイスとスワップチェーンの作成
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = Game::WINDOW_WIDTH;
    sd.BufferDesc.Height = Game::WINDOW_HEIGHT;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL* featureLevel = NULL;

    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevels, 1, D3D11_SDK_VERSION, &sd, &mSwapChain, &mDevice, featureLevel, &mDeviceContext);
}

void Renderer::createRenderTargetView() {
    //レンダーターゲットビューの作成
    ID3D11Texture2D* backBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    mDevice->CreateRenderTargetView(backBuffer, NULL, &mRenderTargetView);
    SAFE_RELEASE(backBuffer);
}

void Renderer::createDepthStencilView() {
    //深度ステンシルビューの作成
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = Game::WINDOW_WIDTH;
    descDepth.Height = Game::WINDOW_HEIGHT;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    mDevice->CreateTexture2D(&descDepth, NULL, &mDepthStencil);
    mDevice->CreateDepthStencilView(mDepthStencil, NULL, &mDepthStencilView);
}

void Renderer::createDepthStencilState() {
    //震度ステンシルステートを作成
    D3D11_DEPTH_STENCIL_DESC dc;
    ZeroMemory(&dc, sizeof(dc));
    dc.DepthEnable = true;
    dc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dc.DepthFunc = D3D11_COMPARISON_LESS;
    dc.StencilEnable = false;
    mDevice->CreateDepthStencilState(&dc, &mDepthStencilState);
    //深度ステンシルステートを適用
    mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 0);
}

void Renderer::createRasterizerState() {
    //ラスタライズ設定
    D3D11_RASTERIZER_DESC rdc;
    ZeroMemory(&rdc, sizeof(rdc));
    rdc.CullMode = D3D11_CULL_FRONT;
    rdc.FillMode = D3D11_FILL_SOLID;

    mDevice->CreateRasterizerState(&rdc, &mRasterizerState);

    rdc.CullMode = D3D11_CULL_BACK;
    mDevice->CreateRasterizerState(&rdc, &mRasterizerStateBack);

    mDeviceContext->RSSetState(mRasterizerStateBack);
}

void Renderer::createBlendState() {
    //アルファブレンド用ブレンドステート作成
    D3D11_BLEND_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
    bd.IndependentBlendEnable = false;
    bd.AlphaToCoverageEnable = false;
    bd.RenderTarget[0].BlendEnable = true;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    mDevice->CreateBlendState(&bd, &mBlendState);

    UINT mask = 0xffffffff;
    mDeviceContext->OMSetBlendState(mBlendState, NULL, mask);
}

D3D11_PRIMITIVE_TOPOLOGY Renderer::toPrimitiveMode(PrimitiveType primitive) {
    static const D3D11_PRIMITIVE_TOPOLOGY primitiveModes[] = {
        D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, //PRIMITIVE_TYPE_POINTLIST = 0
        D3D11_PRIMITIVE_TOPOLOGY_LINELIST, //PRIMITIVE_TYPE_LINELIST = 1
        D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, //PRIMITIVE_TYPE_LINESTRIP = 2
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, //PRIMITIVE_TYPE_TRIANGLE_LIST = 3
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP //PRIMITIVE_TYPE_TRIANGLE_STRIP = 4
    };
    return primitiveModes[static_cast<int>(primitive)];
}
