#include "RenderTargetView.h"
#include "Format.h"
#include "Game.h"
#include "Texture2D.h"
#include "../Device/Renderer.h"

RenderTargetView::RenderTargetView(std::shared_ptr<Renderer> renderer, std::shared_ptr<Texture2D> texture2D, const RenderTargetViewDesc* desc) :
    mRenderer(renderer),
    mRenderTargetView(nullptr) {
    if (desc) {
        renderer->device()->CreateRenderTargetView(texture2D->texture2D(), &toRTVDesc(desc), &mRenderTargetView);
    } else {
        renderer->device()->CreateRenderTargetView(texture2D->texture2D(), nullptr, &mRenderTargetView);
    }
}

RenderTargetView::~RenderTargetView() {
    SAFE_RELEASE(mRenderTargetView);
}

ID3D11RenderTargetView* RenderTargetView::getRenderTaget() const {
    return mRenderTargetView;
}

void RenderTargetView::clearRenderTarget(float r, float g, float b, float a) const {
    const float clearColor[4] = { r, g, b, a };
    if (auto r = mRenderer.lock()) {
        r->deviceContext()->ClearRenderTargetView(mRenderTargetView, clearColor);
    }
}

D3D11_RENDER_TARGET_VIEW_DESC RenderTargetView::toRTVDesc(const RenderTargetViewDesc* desc) const {
    D3D11_RENDER_TARGET_VIEW_DESC rtvd;
    rtvd.Format = toFormat(desc->format);
    rtvd.ViewDimension = toDimension(desc->viewDimension);
    rtvd.Texture2D.MipSlice = desc->texture2D.mipSlice;

    return rtvd;
}

D3D11_RTV_DIMENSION RenderTargetView::toDimension(RTVDimension dimension) const {
    static constexpr D3D11_RTV_DIMENSION dimensions[] {
        D3D11_RTV_DIMENSION_TEXTURE2D
    };

    return dimensions[static_cast<unsigned>(dimension)];
}
