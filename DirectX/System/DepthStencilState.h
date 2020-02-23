#pragma once

#include "DepthStencilDesc.h"
#include "DirectXIncLib.h"

class DepthStencilState {
public:
    DepthStencilState();
    ~DepthStencilState();
    void depthTest(bool value);
    void stencilTest(bool value);
    const DepthStencilDesc& desc() const;

private:
    D3D11_DEPTH_STENCIL_DESC toDepthStencilDesc(const DepthStencilDesc& desc) const;
    D3D11_DEPTH_WRITE_MASK toDepthWriteMask(DepthWriteMask mask) const;
    D3D11_STENCIL_OP toStencilOP(StencilOP stencilOp) const;

private:
    DepthStencilDesc mDesc;
};
