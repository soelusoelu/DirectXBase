#include "DepthStencilState.h"
#include "Game.h"
#include "../Device/Renderer.h"

DepthStencilState::DepthStencilState(std::shared_ptr<Renderer> renderer) :
    mRenderer(renderer),
    mDesc() {
    depthTest(true);
}

DepthStencilState::~DepthStencilState() = default;

void DepthStencilState::depthTest(bool value) {
    if (auto r = mRenderer.lock()) {
        mDesc.depthEnable = value;

        ID3D11DepthStencilState* depthStencilState;

        r->device()->CreateDepthStencilState(&toDepthStencilDesc(mDesc), &depthStencilState);

        r->deviceContext()->OMSetDepthStencilState(depthStencilState, 0);

        SAFE_RELEASE(depthStencilState);
    }
}

const DepthStencilDesc& DepthStencilState::desc() const {
    return mDesc;
}

D3D11_DEPTH_STENCIL_DESC DepthStencilState::toDepthStencilDesc(const DepthStencilDesc& desc) const {
    D3D11_DEPTH_STENCIL_DESC dsd;
    dsd.DepthEnable = desc.depthEnable;
    dsd.DepthWriteMask = toDepthWriteMask(desc.depthWriteMask);
    dsd.DepthFunc = toComparisonFunc(desc.depthFunc);
    dsd.StencilEnable = desc.stencilEnable;
    dsd.StencilReadMask = desc.stencilReadMask;
    dsd.StencilWriteMask = desc.stencilWriteMask;
    dsd.FrontFace.StencilFailOp = toStencilOP(desc.frontFace.stencilFailOp);
    dsd.FrontFace.StencilDepthFailOp = toStencilOP(desc.frontFace.stencilDepthFailOp);
    dsd.FrontFace.StencilPassOp = toStencilOP(desc.frontFace.stencilPassOp);
    dsd.FrontFace.StencilFunc = toComparisonFunc(desc.frontFace.stencilFunc);
    dsd.BackFace.StencilFailOp = toStencilOP(desc.backFace.stencilFailOp);
    dsd.BackFace.StencilDepthFailOp = toStencilOP(desc.backFace.stencilDepthFailOp);
    dsd.BackFace.StencilPassOp = toStencilOP(desc.backFace.stencilPassOp);
    dsd.BackFace.StencilFunc = toComparisonFunc(desc.backFace.stencilFunc);

    return dsd;
}

D3D11_DEPTH_WRITE_MASK DepthStencilState::toDepthWriteMask(const DepthWriteMask& mask) const {
    static constexpr D3D11_DEPTH_WRITE_MASK masks[]{
        D3D11_DEPTH_WRITE_MASK_ZERO,
        D3D11_DEPTH_WRITE_MASK_ALL
    };
    return masks[static_cast<unsigned>(mask)];
}

D3D11_COMPARISON_FUNC DepthStencilState::toComparisonFunc(const ComparisonFunc& func) const {
    static constexpr D3D11_COMPARISON_FUNC funcs[]{
        D3D11_COMPARISON_NEVER,
        D3D11_COMPARISON_LESS,
        D3D11_COMPARISON_EQUAL,
        D3D11_COMPARISON_LESS_EQUAL,
        D3D11_COMPARISON_GREATER,
        D3D11_COMPARISON_NOT_EQUAL,
        D3D11_COMPARISON_GREATER_EQUAL,
        D3D11_COMPARISON_ALWAYS
    };
    return funcs[static_cast<unsigned>(func)];
}

D3D11_STENCIL_OP DepthStencilState::toStencilOP(const StencilOP& stencilOp) const {
    static constexpr D3D11_STENCIL_OP stencilOps[]{
        D3D11_STENCIL_OP_KEEP,
        D3D11_STENCIL_OP_ZERO,
        D3D11_STENCIL_OP_REPLACE,
        D3D11_STENCIL_OP_INCR_SAT,
        D3D11_STENCIL_OP_DECR_SAT,
        D3D11_STENCIL_OP_INVERT,
        D3D11_STENCIL_OP_INCR,
        D3D11_STENCIL_OP_DECR
    };
    return stencilOps[static_cast<unsigned>(stencilOp)];
}
