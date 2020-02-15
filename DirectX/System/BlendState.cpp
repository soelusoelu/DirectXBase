#include "BlendState.h"
#include "Game.h"
#include "../Device/Renderer.h"

BlendState::BlendState(std::shared_ptr<Renderer> renderer) :
    mRenderer(renderer),
    mDesc() {
    setBlendState(mDesc);
}

BlendState::~BlendState() = default;

void BlendState::setBlendState(const BlendDesc& desc, unsigned renderTarget) {
    if (auto r = mRenderer.lock()) {
        mDesc = desc;

        ID3D11BlendState* blend;

        r->device()->CreateBlendState(&toBlendDesc(desc, renderTarget), &blend);

        unsigned mask = 0xffffffff;
        r->deviceContext()->OMSetBlendState(blend, nullptr, mask);

        SAFE_RELEASE(blend);
    }
}

const BlendDesc& BlendState::desc() const {
    return mDesc;
}

D3D11_BLEND_DESC BlendState::toBlendDesc(const BlendDesc& desc, unsigned renderTarget) const {
    D3D11_BLEND_DESC bd;
    bd.AlphaToCoverageEnable = desc.alphaToCoverageEnable;
    bd.IndependentBlendEnable = desc.independentBlendEnable;
    bd.RenderTarget[renderTarget].BlendEnable = desc.renderTarget.blendEnable;
    bd.RenderTarget[renderTarget].SrcBlend = toBlend(desc.renderTarget.srcBlend);
    bd.RenderTarget[renderTarget].DestBlend = toBlend(desc.renderTarget.destBlend);
    bd.RenderTarget[renderTarget].BlendOp = toBlendOP(desc.renderTarget.blendOp);
    bd.RenderTarget[renderTarget].SrcBlendAlpha = toBlend(desc.renderTarget.srcBlendAlpha);
    bd.RenderTarget[renderTarget].DestBlendAlpha = toBlend(desc.renderTarget.destBlendAlpha);
    bd.RenderTarget[renderTarget].BlendOpAlpha = toBlendOP(desc.renderTarget.blendOpAlpha);
    bd.RenderTarget[renderTarget].RenderTargetWriteMask = toColorWriteEnable(desc.renderTarget.renderTargetWriteMask);

    return bd;
}

D3D11_BLEND BlendState::toBlend(const Blend& blend) const {
    static constexpr D3D11_BLEND blends[]{
        D3D11_BLEND_ZERO,
        D3D11_BLEND_ONE,
        D3D11_BLEND_SRC_COLOR,
        D3D11_BLEND_INV_SRC_COLOR,
        D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND_INV_SRC_ALPHA,
        D3D11_BLEND_DEST_ALPHA,
        D3D11_BLEND_INV_DEST_ALPHA,
        D3D11_BLEND_DEST_COLOR,
        D3D11_BLEND_INV_DEST_COLOR,
        D3D11_BLEND_SRC_ALPHA_SAT,
        D3D11_BLEND_BLEND_FACTOR,
        D3D11_BLEND_INV_BLEND_FACTOR,
        D3D11_BLEND_SRC1_COLOR,
        D3D11_BLEND_INV_SRC1_COLOR,
        D3D11_BLEND_SRC1_ALPHA,
        D3D11_BLEND_INV_SRC1_ALPHA
    };
    return blends[static_cast<unsigned>(blend)];
}

D3D11_BLEND_OP BlendState::toBlendOP(const BlendOP& blendOp) const {
    static constexpr D3D11_BLEND_OP blendOps[]{
        D3D11_BLEND_OP_ADD,
        D3D11_BLEND_OP_SUBTRACT,
        D3D11_BLEND_OP_REV_SUBTRACT,
        D3D11_BLEND_OP_MIN,
        D3D11_BLEND_OP_MAX
    };
    return blendOps[static_cast<unsigned>(blendOp)];
}

unsigned BlendState::toColorWriteEnable(const ColorWriteEnable& color) const {
    static constexpr unsigned colors[]{
        D3D11_COLOR_WRITE_ENABLE_RED,
        D3D11_COLOR_WRITE_ENABLE_GREEN,
        D3D11_COLOR_WRITE_ENABLE_BLUE,
        D3D11_COLOR_WRITE_ENABLE_ALPHA,
        D3D11_COLOR_WRITE_ENABLE_ALL
    };
    return colors[static_cast<unsigned>(color)];
}
