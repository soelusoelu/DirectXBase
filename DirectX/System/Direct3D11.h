#pragma once

#include "DirectXIncLib.h"

class Direct3D11 {
public:
    Direct3D11();
    ~Direct3D11();
    bool init(const HWND& hWnd);
    void clear();
    HRESULT present();
    ID3D11Device* device() const;
    ID3D11DeviceContext* deviceContext() const;


private:
    void createSwapChain(const HWND& hWnd);
    void createRenderTargetView();
    void createDepthStencilView();
    void setRenderTargets();
    void createDepthStencilState();
    void setViewports();
    void createRasterizerState();
    void createBlendState();

private:
    ID3D11Device* mDevice;
    ID3D11DeviceContext* mDeviceContext;
    IDXGISwapChain* mSwapChain;
    ID3D11RenderTargetView* mRenderTargetView;
    ID3D11DepthStencilView* mDepthStencilView;
    ID3D11Texture2D* mDepthStencil;
    ID3D11DepthStencilState* mDepthStencilState;
    ID3D11RasterizerState* mRasterizerState; //‘O–Ê—p
    ID3D11RasterizerState* mRasterizerStateBack; //”w–Ê—p
    ID3D11BlendState* mBlendState;
};

