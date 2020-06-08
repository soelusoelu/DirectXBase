#pragma once

#include "BlendDesc.h"
#include "SystemInclude.h"
#include <memory>

class BlendState {
public:
    BlendState();
    ~BlendState();
    void setBlendState(const BlendDesc& desc, unsigned renderTarget = 0);
    const BlendDesc& desc() const;

private:
    D3D11_BLEND_DESC toBlendDesc(const BlendDesc& desc, unsigned renderTarget) const;
    D3D11_BLEND toBlend(const Blend& blend) const;
    D3D11_BLEND_OP toBlendOP(const BlendOP& blendOp) const;
    unsigned toColorWriteEnable(const ColorWriteEnable& color) const;

    //コピー禁止
    BlendState(const BlendState&) = delete;
    BlendState& operator=(const BlendState&) = delete;

private:
    BlendDesc mDesc;
};
