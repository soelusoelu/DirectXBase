#pragma once

#include "BufferDesc.h"
#include "SamplerDesc.h"
#include "SubResourceDesc.h"
#include "SystemInclude.h"
#include "Texture2DDesc.h"
#include "../Utility/Singleton.h"
#include <memory>

enum class PrimitiveType {
    PRIMITIVE_TYPE_POINT_LIST,
    PRIMITIVE_TYPE_LINE_LIST,
    PRIMITIVE_TYPE_LINE_STRIP,
    PRIMITIVE_TYPE_TRIANGLE_LIST,
    PRIMITIVE_TYPE_TRIANGLE_STRIP
};

class BlendState;
class DepthStencilState;
class GBuffer;
class RasterizerState;
class RenderTargetView;

class DirectX {
    friend class Singleton<DirectX>;

public:
    void initialize(const HWND& hWnd);

    ID3D11Device* device() const;
    ID3D11DeviceContext* deviceContext() const;
    std::shared_ptr<BlendState> blendState() const;
    std::shared_ptr<DepthStencilState> depthStencilState() const;
    std::shared_ptr<RasterizerState> rasterizerState() const;

    void setViewport(float width, float height, float x = 0.f, float y = 0.f) const;
    void setRenderTarget() const;
    void setDebugRenderTarget() const;
    void setRenderTargets(ID3D11RenderTargetView* targets[], unsigned numTargets) const;
    void setPrimitive(PrimitiveType primitive) const;

    void draw(unsigned numVertex, unsigned start = 0);
    void drawIndexed(unsigned numIndices, unsigned startIndex = 0, int startVertex = 0);

    void clearRenderTarget(float r = 0.f, float g = 0.f, float b = 1.f, float a = 1.f) const;
    void clearDepthStencilView(bool depth = true, bool stencil = false);
    void present();

private:
    DirectX();
    ~DirectX();
    void createDeviceAndSwapChain(const HWND& hWnd);
    void createRenderTargetView();
    void createDepthStencilView();
    D3D11_PRIMITIVE_TOPOLOGY toPrimitiveMode(PrimitiveType primitive) const;

private:
    ID3D11Device* mDevice;
    ID3D11DeviceContext* mDeviceContext;
    IDXGISwapChain* mSwapChain;
    ID3D11DepthStencilView* mDepthStencilView;

    std::unique_ptr<RenderTargetView> mRenderTargetView;
    std::unique_ptr<RenderTargetView> mDebugRenderTargetView;
    std::shared_ptr<BlendState> mBlendState;
    std::shared_ptr<DepthStencilState> mDepthStencilState;
    std::shared_ptr<RasterizerState> mRasterizerState;
};
