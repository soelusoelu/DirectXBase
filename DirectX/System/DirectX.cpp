#include "DirectX.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "Game.h"
#include "RasterizerState.h"
#include "RenderTargetView.h"
#include "Texture2D.h"

DirectX::DirectX() :
    mDevice(nullptr),
    mDeviceContext(nullptr),
    mSwapChain(nullptr),
    mDepthStencilView(nullptr),
    mRenderTargetView(nullptr),
    mBlendState(nullptr),
    mDepthStencilState(nullptr),
    mRasterizerState(nullptr) {
}

DirectX::~DirectX() {
    SAFE_RELEASE(mDepthStencilView);
    SAFE_RELEASE(mSwapChain);
    SAFE_RELEASE(mDeviceContext);
    SAFE_RELEASE(mDevice);
}

void DirectX::initialize(const HWND& hWnd) {
    createDeviceAndSwapChain(hWnd);
    createDepthStencilView();
    createRenderTargetView();
    setRenderTarget();
    setViewport(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);

    mBlendState = std::make_shared<BlendState>();
    mDepthStencilState = std::make_shared<DepthStencilState>();
    mRasterizerState = std::make_shared<RasterizerState>();
}

ID3D11Device* DirectX::device() const {
    return mDevice;
}

ID3D11DeviceContext* DirectX::deviceContext() const {
    return mDeviceContext;
}

std::shared_ptr<BlendState> DirectX::blendState() const {
    return mBlendState;
}

std::shared_ptr<DepthStencilState> DirectX::depthStencilState() const {
    return mDepthStencilState;
}

std::shared_ptr<RasterizerState> DirectX::rasterizerState() const {
    return mRasterizerState;
}

void DirectX::setViewport(float width, float height, float x, float y) const {
    //ビューポートの設定
    D3D11_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
    vp.TopLeftX = x;
    vp.TopLeftY = y;
    mDeviceContext->RSSetViewports(1, &vp);
}

void DirectX::setRenderTarget() const {
    auto rt = mRenderTargetView->getRenderTarget();
    mDeviceContext->OMSetRenderTargets(1, &rt, mDepthStencilView);
}

void DirectX::setDebugRenderTarget() const {
    auto rt = mDebugRenderTargetView->getRenderTarget();
    mDeviceContext->OMSetRenderTargets(1, &rt, mDepthStencilView);
}

void DirectX::setRenderTargets(ID3D11RenderTargetView* targets[], unsigned numTargets) const {
    mDeviceContext->OMSetRenderTargets(numTargets, targets, mDepthStencilView);
}

void DirectX::setPrimitive(PrimitiveType primitive) const {
    mDeviceContext->IASetPrimitiveTopology(toPrimitiveMode(primitive));
}

void DirectX::draw(unsigned numVertex, unsigned start) {
    mDeviceContext->Draw(numVertex, start);
}

void DirectX::drawIndexed(unsigned numIndices, unsigned startIndex, int startVertex) {
    mDeviceContext->DrawIndexed(numIndices, startIndex, startVertex);
}

void DirectX::clearRenderTarget(float r, float g, float b, float a) const {
    mRenderTargetView->clearRenderTarget(r, g, b, a);
    mDebugRenderTargetView->clearRenderTarget(0.1f, 0.1f, 0.1f, 1.f);
}

void DirectX::clearDepthStencilView(bool depth, bool stencil) {
    unsigned mask = 0;
    if (depth) {
        mask |= D3D11_CLEAR_DEPTH;
    }
    if (stencil) {
        mask |= D3D11_CLEAR_STENCIL;
    }
    mDeviceContext->ClearDepthStencilView(mDepthStencilView, mask, 1.f, 0); //深度バッファクリア
}

void DirectX::present() {
    mSwapChain->Present(0, 0);
}

void DirectX::createDeviceAndSwapChain(const HWND& hWnd) {
    // デバイスとスワップチェーンの作成
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = Game::WINDOW_DEBUG_WIDTH;
    sd.BufferDesc.Height = Game::WINDOW_DEBUG_HEIGHT;
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

void DirectX::createRenderTargetView() {
    ID3D11Texture2D* backBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    auto tex = std::make_shared<Texture2D>(backBuffer);
    mRenderTargetView = std::make_unique<RenderTargetView>(tex);

    mDebugRenderTargetView = std::make_unique<RenderTargetView>(tex);
}

void DirectX::createDepthStencilView() {
    Texture2DDesc desc;
    desc.width = Game::WINDOW_DEBUG_WIDTH;
    desc.height = Game::WINDOW_DEBUG_HEIGHT;
    desc.format = Format::FORMAT_D24_UNORM_S8_UINT;
    desc.bindFlags = static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_DEPTH_STENCIL);

    auto tex = std::make_shared<Texture2D>(desc);
    mDevice->CreateDepthStencilView(tex->texture2D(), nullptr, &mDepthStencilView);
}

D3D11_PRIMITIVE_TOPOLOGY DirectX::toPrimitiveMode(PrimitiveType primitive) const {
    static const D3D11_PRIMITIVE_TOPOLOGY primitiveModes[] = {
        D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, //PRIMITIVE_TYPE_POINTLIST = 0
        D3D11_PRIMITIVE_TOPOLOGY_LINELIST, //PRIMITIVE_TYPE_LINELIST = 1
        D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, //PRIMITIVE_TYPE_LINESTRIP = 2
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, //PRIMITIVE_TYPE_TRIANGLE_LIST = 3
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP //PRIMITIVE_TYPE_TRIANGLE_STRIP = 4
    };
    return primitiveModes[static_cast<int>(primitive)];
}
