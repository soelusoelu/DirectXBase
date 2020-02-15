#include "Renderer.h"
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
#include "../System/DirectXIncLib.h"
#include "../System/GBuffer.h"
#include "../System/RasterizerState.h"
#include "../System/SubResourceDesc.h"
#include "../System/Texture.h"
#include "../System/VertexStreamDesc.h"
#include "../System/ViewportDesc.h"

Renderer::Renderer(const HWND& hWnd) :
    mDevice(nullptr),
    mDeviceContext(nullptr),
    mSwapChain(nullptr),
    mRenderTargetView(nullptr),
    mDepthStencilView(nullptr),
    mSoundBase(std::make_unique<SoundBase>()),
    mBlendState(nullptr),
    mDepthStencilState(nullptr),
    mRasterizerState(nullptr),
    mGBuffer(std::make_shared<GBuffer>()),
    mPointLight(std::make_shared<PointLight>()),
    mAmbientLight(Vector3(0.4f, 0.4f, 0.4f)) {
    createDeviceAndSwapChain(hWnd);
    createRenderTargetView();
    createDepthStencilView();
    setDefaultRenderTarget();
    ViewportDesc desc;
    desc.width = Game::WINDOW_WIDTH;
    desc.height = Game::WINDOW_HEIGHT;
    setViewport(desc);
}

Renderer::~Renderer() {
    mShaders.clear();
    mTextures.clear();
    mSounds.clear();
    mMeshLoaders.clear();

    SAFE_RELEASE(mDepthStencilView);
    SAFE_RELEASE(mRenderTargetView);
    SAFE_RELEASE(mSwapChain);
    SAFE_RELEASE(mDeviceContext);
    SAFE_RELEASE(mDevice);
}

void Renderer::initialize() {
    mBlendState = std::make_shared<BlendState>(shared_from_this());
    mDepthStencilState = std::make_shared<DepthStencilState>(shared_from_this());
    mRasterizerState = std::make_shared<RasterizerState>(shared_from_this());
    mGBuffer->create(shared_from_this());
    mPointLight->initialize(shared_from_this());
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

void Renderer::setPrimitive(PrimitiveType primitive) const {
    mDeviceContext->IASetPrimitiveTopology(toPrimitiveMode(primitive));
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
    mPointLight->mesh->getMeshData()->setVertexBuffer(sizeof(MeshVertex));
    //プリミティブ指定
    setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_LIST);
    static constexpr unsigned numGBuffer = static_cast<unsigned>(GBuffer::Type::NUM_GBUFFER_TEXTURES);
    for (size_t i = 0; i < numGBuffer; i++) {
        auto sr = mGBuffer->getShaderResource(i);
        mDeviceContext->PSSetShaderResources(i, 1, &sr);
    }
    //サンプラーをセット
    auto s = mGBuffer->getSampler();
    mDeviceContext->PSSetSamplers(0, 1, &s);
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

void Renderer::renderToTexture() {
    //各テクスチャをレンダーターゲットに設定
    static constexpr unsigned numGBuffer = static_cast<unsigned>(GBuffer::Type::NUM_GBUFFER_TEXTURES);
    ID3D11RenderTargetView* views[numGBuffer];
    for (size_t i = 0; i < numGBuffer; i++) {
        views[i] = mGBuffer->getRenderTarget(i);
    }
    setRenderTargets(views, numGBuffer);
    //クリア
    static constexpr float clearColor[4] = { 0.f, 0.f, 1.f, 1.f }; //クリア色作成 RGBAの順
    for (size_t i = 0; i < numGBuffer; i++) {
        mDeviceContext->ClearRenderTargetView(views[i], clearColor); //画面クリア
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
    setDefaultRenderTarget();
    //クリア
    clear();

    //使用するシェーダーは、テクスチャーを参照するシェーダー
    mGBuffer->shader()->setVSShader();
    mGBuffer->shader()->setPSShader();
    //1パス目で作成したテクスチャー3枚をセット
    static constexpr unsigned numGBuffer = static_cast<unsigned>(GBuffer::Type::NUM_GBUFFER_TEXTURES);
    for (size_t i = 0; i < numGBuffer; i++) {
        auto sr = mGBuffer->getShaderResource(i);
        mDeviceContext->PSSetShaderResources(i, 1, &sr);
    }
    //サンプラーをセット
    auto s = mGBuffer->getSampler();
    mDeviceContext->PSSetSamplers(0, 1, &s);

    D3D11_MAPPED_SUBRESOURCE pData;
    if (mGBuffer->shader()->map(&pData)) {
        GBufferShaderConstantBuffer cb;
        cb.lightDir = DirectionalLight::direction;
        cb.cameraPos = camera->getPosition();
        cb.ambientLight = mAmbientLight;

        memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(cb));
        mGBuffer->shader()->unmap();
    }
    //スクリーンサイズのポリゴンをレンダー
    setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_STRIP);
    //バーテックスバッファーをセット
    VertexStreamDesc stream;
    stream.sharedBuffer = mGBuffer->vertexBuffer();
    stream.offset = 0;
    stream.stride = sizeof(MeshVertex);
    setVertexBuffer(&stream);
    //デプステスト無効化
    mDepthStencilState->depthTest(false);

    draw(4);
}

void Renderer::draw(unsigned numVertex, unsigned start) {
    mDeviceContext->Draw(numVertex, start);
}

void Renderer::drawIndexed(unsigned numIndices, unsigned startIndex, int startVertex) {
    mDeviceContext->DrawIndexed(numIndices, startIndex, startVertex);
}

void Renderer::clear(float r, float g, float b, float a, bool depth, bool stencil) {
    clearRenderTarget(r, g, b, a);
    clearDepthStencilView(depth, stencil);
}

void Renderer::clearRenderTarget(float r, float g, float b, float a) {
    const float clearColor[4] = { r, g, b, a }; //クリア色作成 RGBAの順
    mDeviceContext->ClearRenderTargetView(mRenderTargetView, clearColor); //画面クリア
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
    ID3D11Texture2D* depthStencil;
    mDevice->CreateTexture2D(&descDepth, NULL, &depthStencil);
    mDevice->CreateDepthStencilView(depthStencil, NULL, &mDepthStencilView);

    SAFE_RELEASE(depthStencil);
}

void Renderer::setRenderTargets(ID3D11RenderTargetView* targets[], unsigned numTargets) {
    mDeviceContext->OMSetRenderTargets(numTargets, targets, mDepthStencilView);
}

void Renderer::setDefaultRenderTarget() {
    mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
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
