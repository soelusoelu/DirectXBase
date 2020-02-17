#pragma once

#include "DirectXIncLib.h"
#include "SamplerDesc.h"
#include <memory>

class Renderer;

class Sampler {
public:
    Sampler(std::shared_ptr<Renderer> renderer, const SamplerDesc& desc);
    ~Sampler();
    void setVSSamplers(unsigned start = 0, unsigned numSamplers = 1);
    void setPSSamplers(unsigned start = 0, unsigned numSamplers = 1);
    const SamplerDesc& desc() const;

private:
    D3D11_SAMPLER_DESC toSamplerDesc(const SamplerDesc& desc) const;
    D3D11_FILTER toFilter(Filter filter) const;
    D3D11_TEXTURE_ADDRESS_MODE toAddress(TextureAddressMode mode) const;

private:
    std::weak_ptr<Renderer> mRenderer;
    ID3D11SamplerState* mSampler;
    SamplerDesc mDesc;
};
