#pragma once

#include "DepthStencilDesc.h"
#include "DirectXIncLib.h"
#include <memory>

class Renderer;

class DepthStencilState {
public:
    DepthStencilState(std::shared_ptr<Renderer> renderer);
    ~DepthStencilState();
    void depthTest(bool value);
    const DepthStencilDesc& desc() const;

private:
    D3D11_DEPTH_STENCIL_DESC toDepthStencilDesc(const DepthStencilDesc& desc) const;
    D3D11_DEPTH_WRITE_MASK toDepthWriteMask(const DepthWriteMask& mask) const;
    D3D11_COMPARISON_FUNC toComparisonFunc(const ComparisonFunc& func) const;
    D3D11_STENCIL_OP toStencilOP(const StencilOP& stencilOp) const;

private:
    std::weak_ptr<Renderer> mRenderer;
    DepthStencilDesc mDesc;
};
