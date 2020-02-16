#pragma once

#include "DirectXIncLib.h"
#include "ShaderResourceViewDesc.h"
#include <memory>

class Renderer;
class Texture2D;

class ShaderResourceView {
public:
    ShaderResourceView(std::shared_ptr<Renderer> renderer, std::shared_ptr<Texture2D> texture2D, const ShaderResourceViewDesc* desc = nullptr);
    ShaderResourceView(std::shared_ptr<Renderer> renderer, ID3D11ShaderResourceView* view);
    ~ShaderResourceView();
    void setVSShaderResources(unsigned start = 0, unsigned numViews = 1);
    void setPSShaderResources(unsigned start = 0, unsigned numViews = 1);

private:
    D3D11_SHADER_RESOURCE_VIEW_DESC toSRVDesc(const ShaderResourceViewDesc* desc) const;
    D3D11_SRV_DIMENSION toDimension(SRVDimension dimension) const;

private:
    std::weak_ptr<Renderer> mRenderer;
    ID3D11ShaderResourceView* mShaderResourceView;
};
