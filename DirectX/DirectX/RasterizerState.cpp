#include "RasterizerState.h"
#include "DirectX.h"
#include "../System/GlobalFunction.h"

RasterizerState::RasterizerState() :
    mDesc() {
    setRasterizerState(mDesc);
}

RasterizerState::~RasterizerState() = default;

void RasterizerState::setRasterizerState(const RasterizerDesc & desc) {
    mDesc = desc;

    ID3D11RasterizerState* rasterizer;

    Singleton<DirectX>::instance().device()->CreateRasterizerState(&toRasterizerDesc(mDesc), &rasterizer);

    Singleton<DirectX>::instance().deviceContext()->RSSetState(rasterizer);

    safeRelease(rasterizer);
}

const RasterizerDesc& RasterizerState::desc() const {
    return mDesc;
}

D3D11_RASTERIZER_DESC RasterizerState::toRasterizerDesc(const RasterizerDesc & desc) const {
    D3D11_RASTERIZER_DESC rd;
    rd.FillMode = toFillMode(desc.fillMode);
    rd.CullMode = toCullMode(desc.cullMode);
    rd.FrontCounterClockwise = desc.frontCounterClockwise;
    rd.DepthBias = desc.depthBias;
    rd.DepthBiasClamp = desc.depthBiasClamp;
    rd.SlopeScaledDepthBias = desc.slopeScaledDepthBias;
    rd.DepthClipEnable = desc.depthClipEnable;
    rd.ScissorEnable = desc.scissorEnable;
    rd.MultisampleEnable = desc.multisampleEnable;
    rd.AntialiasedLineEnable = desc.antialiasedLineEnable;

    return rd;
}

D3D11_FILL_MODE RasterizerState::toFillMode(const FillMode & mode) const {
    static constexpr D3D11_FILL_MODE modes[]{
        D3D11_FILL_WIREFRAME,
        D3D11_FILL_SOLID
    };
    return modes[static_cast<unsigned>(mode)];
}

D3D11_CULL_MODE RasterizerState::toCullMode(const CullMode & mode) const {
    static constexpr D3D11_CULL_MODE modes[]{
        D3D11_CULL_NONE,
        D3D11_CULL_FRONT,
        D3D11_CULL_BACK
    };
    return modes[static_cast<unsigned>(mode)];
}
