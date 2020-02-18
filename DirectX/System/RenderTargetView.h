#pragma once

#include "DirectXIncLib.h"
#include "RenderTargetViewDesc.h"
#include <memory>

class Renderer;
class Texture2D;

class RenderTargetView {
public:
    RenderTargetView(std::shared_ptr<Renderer> renderer, std::shared_ptr<Texture2D> texture2D, const RenderTargetViewDesc* desc = nullptr);
    ~RenderTargetView();
    ID3D11RenderTargetView* getRenderTaget() const;
    void clearRenderTarget(float r = 0.f, float g = 0.f, float b = 1.f, float a = 1.f) const;

private:
    D3D11_RENDER_TARGET_VIEW_DESC toRTVDesc(const RenderTargetViewDesc* desc) const;
    D3D11_RTV_DIMENSION toDimension(RTVDimension dimension) const;

private:
    std::weak_ptr<Renderer> mRenderer;
    ID3D11RenderTargetView* mRenderTargetView;
};
