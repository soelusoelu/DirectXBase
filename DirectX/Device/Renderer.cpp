#include "Renderer.h"
#include "AssetsManager.h"
#include "Sound.h"
#include "../Camera/Camera.h"
#include "../Component/PointLightComponent.h"
#include "../Light/DirectionalLight.h"
#include "../Light/PointLight.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/MeshLoader.h"
#include "../Shader/Shader.h"
#include "../System/BlendState.h"
#include "../System/Buffer.h"
#include "../System/DepthStencilState.h"
#include "../System/Format.h"
#include "../System/GBuffer.h"
#include "../System/IndexBuffer.h"
#include "../System/RenderTargetView.h"
#include "../System/RenderTargetViewDesc.h"
#include "../System/Sampler.h"
#include "../System/ShaderResourceView.h"
#include "../System/SubResourceDesc.h"
#include "../System/RasterizerState.h"
#include "../System/Texture.h"
#include "../System/Texture2D.h"
#include "../System/VertexBuffer.h"
#include "../Utility/LevelLoader.h"

Renderer::Renderer(const HWND& hWnd) :
    mDevice(nullptr),
    mDeviceContext(nullptr),
    mSwapChain(nullptr),
    mDepthStencilView(nullptr),
    mRenderTargetView(nullptr),
    mBlendState(nullptr),
    mDepthStencilState(nullptr),
    mRasterizerState(nullptr),
    mAssetsManager(nullptr),
    mGBuffer(std::make_shared<GBuffer>()),
    mPointLight(std::make_shared<PointLight>()),
    mAmbientLight(Vector3::zero) {
    createDeviceAndSwapChain(hWnd);
    ViewportDesc desc;
    desc.width = Game::WINDOW_WIDTH;
    desc.height = Game::WINDOW_HEIGHT;
    setViewport(desc);
}

Renderer::~Renderer() {
    SAFE_RELEASE(mDepthStencilView);
    SAFE_RELEASE(mSwapChain);
    SAFE_RELEASE(mDeviceContext);
    SAFE_RELEASE(mDevice);
}

void Renderer::initialize() {
    mBlendState = std::make_shared<BlendState>(shared_from_this());
    mDepthStencilState = std::make_shared<DepthStencilState>(shared_from_this());
    mRasterizerState = std::make_shared<RasterizerState>(shared_from_this());
    mAssetsManager = std::make_shared<AssetsManager>(shared_from_this());
    mGBuffer->create(shared_from_this());
    mPointLight->initialize(shared_from_this());

    createDepthStencilView();
    createRenderTargetView();
    setRenderTarget();

    //ディレクショナルライトと環境光の読み込み
    LevelLoader::loadLevel(shared_from_this(), "globalLight.json");
}

ID3D11Device* Renderer::device() const {
    return mDevice;
}

ID3D11DeviceContext* Renderer::deviceContext() const {
    return mDeviceContext;
}

std::shared_ptr<BlendState> Renderer::blendState() const {
    return mBlendState;
}

std::shared_ptr<DepthStencilState> Renderer::depthStencilState() const {
    return mDepthStencilState;
}

std::shared_ptr<RasterizerState> Renderer::rasterizerState() const {
    return mRasterizerState;
}

std::shared_ptr<AssetsManager> Renderer::getAssetsManager() const {
    return mAssetsManager;
}

std::shared_ptr<Buffer> Renderer::createBuffer(const BufferDesc& desc, const SubResourceDesc* data) {
    return std::make_shared<Buffer>(shared_from_this(), desc, data);
}

std::shared_ptr<VertexBuffer> Renderer::createVertexBuffer(const BufferDesc& desc, const SubResourceDesc* data) {
    return std::make_shared<VertexBuffer>(shared_from_this(), desc, data);
}

std::shared_ptr<IndexBuffer> Renderer::createIndexBuffer(const BufferDesc& desc, const SubResourceDesc* data) {
    return std::make_shared<IndexBuffer>(shared_from_this(), desc, data);
}

std::shared_ptr<Texture2D> Renderer::createTexture2D(const Texture2DDesc& desc, const SubResourceDesc* data) const {
    return std::make_shared<Texture2D>(mDevice, desc, data);
}

std::shared_ptr<Sampler> Renderer::createSamplerState(const SamplerDesc& desc) {
    return std::make_shared<Sampler>(shared_from_this(), desc);
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

void Renderer::setPrimitive(PrimitiveType primitive) const {
    mDeviceContext->IASetPrimitiveTopology(toPrimitiveMode(primitive));
}

void Renderer::addPointLight(PointLightComponent* light) {
    mPointLigths.emplace_back(light);
}

void Renderer::removePointLight(PointLightComponent* light) {
    auto itr = std::find(mPointLigths.begin(), mPointLigths.end(), light);
    mPointLigths.erase(itr);
}

void Renderer::drawPointLights(std::shared_ptr<Camera> camera) {
    if (mPointLigths.empty()) {
        return;
    }

    auto shader = mPointLight->shader;
    //使用するシェーダーの登録
    shader->setVSShader();
    shader->setPSShader();
    //このコンスタントバッファーを使うシェーダーの登録
    shader->setVSConstantBuffers();
    shader->setPSConstantBuffers();
    //頂点インプットレイアウトをセット
    shader->setInputLayout();
    //バーテックスバッファーをセット
    mPointLight->mesh->getMeshData()->setVertexBuffer();
    //プリミティブ指定
    setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_LIST);
    static constexpr unsigned numGBuffer = static_cast<unsigned>(GBuffer::Type::NUM_GBUFFER_TEXTURES);
    for (size_t i = 0; i < numGBuffer; i++) {
        mGBuffer->getShaderResourceView(i)->setPSShaderResources(i);
    }
    //サンプラーをセット
    mGBuffer->getSampler()->setPSSamplers();
    //デプステスト有効化
    mDepthStencilState->depthTest(true);
    //加算合成
    BlendDesc bd;
    bd.renderTarget.srcBlend = Blend::ONE;
    bd.renderTarget.destBlend = Blend::ONE;
    mBlendState->setBlendState(bd);

    for (const auto& p : mPointLigths) {
        p->draw(mPointLight, camera);
    }
}

void Renderer::setAmbientLight(const Vector3& ambient) {
    mAmbientLight = ambient;
}

void Renderer::renderToTexture() {
    //各テクスチャをレンダーターゲットに設定
    setGBufferRenderTargets();
    //クリア
    static constexpr unsigned numGBuffer = static_cast<unsigned>(GBuffer::Type::NUM_GBUFFER_TEXTURES);
    for (size_t i = 0; i < numGBuffer; i++) {
        mGBuffer->getRenderTarget(i)->clearRenderTarget();
    }
    clearDepthStencilView();
    //デプステスト有効化
    mDepthStencilState->depthTest(true);
    //通常合成
    BlendDesc bd;
    bd.renderTarget.srcBlend = Blend::SRC_ALPHA;
    bd.renderTarget.destBlend = Blend::INV_SRC_ALPHA;
    mBlendState->setBlendState(bd);
}

void Renderer::renderFromTexture(std::shared_ptr<Camera> camera) {
    //レンダーターゲットを通常に戻す
    setRenderTarget();
    //クリア
    mRenderTargetView->clearRenderTarget();
    clearDepthStencilView();

    //使用するシェーダーは、テクスチャーを参照するシェーダー
    mGBuffer->shader()->setVSShader();
    mGBuffer->shader()->setPSShader();
    //コンスタントバッファの登録
    mGBuffer->shader()->setPSConstantBuffers();
    //1パス目で作成したテクスチャー3枚をセット
    static constexpr unsigned numGBuffer = static_cast<unsigned>(GBuffer::Type::NUM_GBUFFER_TEXTURES);
    for (size_t i = 0; i < numGBuffer; i++) {
        mGBuffer->getShaderResourceView(i)->setPSShaderResources(i);
    }
    //サンプラーをセット
    mGBuffer->getSampler()->setPSSamplers();

    MappedSubResourceDesc msrd;
    if (mGBuffer->shader()->map(&msrd)) {
        GBufferShaderConstantBuffer cb;
        cb.dirLightDir = DirectionalLight::direction;
        cb.dirLightColor = DirectionalLight::color;
        cb.cameraPos = camera->getPosition();
        cb.ambientLight = mAmbientLight;

        memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
        mGBuffer->shader()->unmap();
    }
    //スクリーンサイズのポリゴンをレンダー
    setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_STRIP);
    //バーテックスバッファーをセット
    mGBuffer->getVertexBuffer()->setVertexBuffer();
    //インデックスバッファをセット
    mGBuffer->getIndexBuffer()->setIndexBuffer(Format::FORMAT_R16_UINT);
    //デプステスト無効化
    mDepthStencilState->depthTest(false);

    drawIndexed(6);
}

void Renderer::draw(unsigned numVertex, unsigned start) {
    mDeviceContext->Draw(numVertex, start);
}

void Renderer::drawIndexed(unsigned numIndices, unsigned startIndex, int startVertex) {
    mDeviceContext->DrawIndexed(numIndices, startIndex, startVertex);
}

void Renderer::clearRenderTarget(float r, float g, float b, float a) const {
    mRenderTargetView->clearRenderTarget(r, g, b, a);
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
    ID3D11Texture2D* backBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    auto tex = std::make_shared<Texture2D>(backBuffer);
    mRenderTargetView = std::make_unique<RenderTargetView>(shared_from_this(), tex);
}

void Renderer::createDepthStencilView() {
    Texture2DDesc desc;
    desc.width = Game::WINDOW_WIDTH;
    desc.height = Game::WINDOW_HEIGHT;
    desc.format = Format::FORMAT_D24_UNORM_S8_UINT;
    desc.bindFlags = static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_DEPTH_STENCIL);

    auto tex = createTexture2D(desc);
    mDevice->CreateDepthStencilView(tex->texture2D(), nullptr, &mDepthStencilView);
}

void Renderer::setGBufferRenderTargets() const {
    static constexpr unsigned numGBuffer = static_cast<unsigned>(GBuffer::Type::NUM_GBUFFER_TEXTURES);
    ID3D11RenderTargetView* views[numGBuffer];
    for (size_t i = 0; i < numGBuffer; i++) {
        views[i] = mGBuffer->getRenderTarget(i)->getRenderTaget();
    }

    mDeviceContext->OMSetRenderTargets(numGBuffer, views, mDepthStencilView);
}

void Renderer::setRenderTarget() const {
    auto rt = mRenderTargetView->getRenderTaget();
    mDeviceContext->OMSetRenderTargets(1, &rt, mDepthStencilView);
}

D3D11_PRIMITIVE_TOPOLOGY Renderer::toPrimitiveMode(PrimitiveType primitive) const {
    static const D3D11_PRIMITIVE_TOPOLOGY primitiveModes[] = {
        D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, //PRIMITIVE_TYPE_POINTLIST = 0
        D3D11_PRIMITIVE_TOPOLOGY_LINELIST, //PRIMITIVE_TYPE_LINELIST = 1
        D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, //PRIMITIVE_TYPE_LINESTRIP = 2
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, //PRIMITIVE_TYPE_TRIANGLE_LIST = 3
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP //PRIMITIVE_TYPE_TRIANGLE_STRIP = 4
    };
    return primitiveModes[static_cast<int>(primitive)];
}
