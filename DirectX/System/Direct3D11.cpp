#include "Direct3D11.h"
#include "Game.h"

Direct3D11::Direct3D11() {
    ZeroMemory(this, sizeof(Direct3D11));
}

Direct3D11::~Direct3D11() {
    SAFE_RELEASE(mDepthStencilState);
    SAFE_RELEASE(mDepthStencil);
    SAFE_RELEASE(mDepthStencilView);
    SAFE_RELEASE(mRenderTargetView);
    SAFE_RELEASE(mSwapChain);
    SAFE_RELEASE(mRasterizerState);
    SAFE_RELEASE(mRasterizerStateBack);
    SAFE_RELEASE(mBlendState);
    SAFE_RELEASE(mDeviceContext);
    SAFE_RELEASE(mDevice);
}

bool Direct3D11::init(const HWND& hWnd) {
    createSwapChain(hWnd);

    createRenderTargetView();

    createDepthStencilView();

    setRenderTargets();

    createDepthStencilState();

    setViewports();

    createRasterizerState();

    createBlendState();

    return true;
}

void Direct3D11::clear() {
    //画面クリア(実際は単色で画面を塗りつぶす処理)
    float ClearColor[4] = { 0,0,1,1 }; //クリア色作成 RGBAの順
    mDeviceContext->ClearRenderTargetView(mRenderTargetView, ClearColor); //画面クリア
    mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); //深度バッファクリア
}

HRESULT Direct3D11::present() {
    mSwapChain->Present(0, 0); //画面更新(バックバッファをフロントバッファに)
    return S_OK;
}

ID3D11Device* Direct3D11::device() const {
    return mDevice;
}

ID3D11DeviceContext* Direct3D11::deviceContext() const {
    return mDeviceContext;
}

void Direct3D11::createSwapChain(const HWND& hWnd) {
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

void Direct3D11::createRenderTargetView() {
    //レンダーターゲットビューの作成
    ID3D11Texture2D* backBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    mDevice->CreateRenderTargetView(backBuffer, NULL, &mRenderTargetView);
    SAFE_RELEASE(backBuffer);
}

void Direct3D11::createDepthStencilView() {
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

void Direct3D11::setRenderTargets() {
    //レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
    mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void Direct3D11::createDepthStencilState() {
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

void Direct3D11::setViewports() {
    //ビューポートの設定
    D3D11_VIEWPORT vp;
    vp.Width = Game::WINDOW_WIDTH;
    vp.Height = Game::WINDOW_HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    mDeviceContext->RSSetViewports(1, &vp);
}

void Direct3D11::createRasterizerState() {
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

void Direct3D11::createBlendState() {
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
