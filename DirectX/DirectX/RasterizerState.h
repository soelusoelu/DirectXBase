#pragma once

#include "RasterizerDesc.h"
#include "../System/SystemInclude.h"

class RasterizerState {
public:
    RasterizerState();
    ~RasterizerState();
    void setRasterizerState(const RasterizerDesc& desc);
    const RasterizerDesc& desc() const;

private:
    D3D11_RASTERIZER_DESC toRasterizerDesc(const RasterizerDesc& desc) const;
    D3D11_FILL_MODE toFillMode(const FillMode& mode) const;
    D3D11_CULL_MODE toCullMode(const CullMode& mode) const;

private:
    RasterizerDesc mDesc;
};
