#pragma once

#include "DirectXIncLib.h"
#include "RasterizerDesc.h"
#include <memory>

class Renderer;

class RasterizerState {
public:
    RasterizerState(std::shared_ptr<Renderer> renderer);
    ~RasterizerState();
    void setRasterizerState(const RasterizerDesc& desc);
    const RasterizerDesc& desc() const;

private:
    D3D11_RASTERIZER_DESC toRasterizerDesc(const RasterizerDesc& desc) const;
    D3D11_FILL_MODE toFillMode(const FillMode& mode) const;
    D3D11_CULL_MODE toCullMode(const CullMode& mode) const;

private:
    std::weak_ptr<Renderer> mRenderer;
    RasterizerDesc mDesc;
};
