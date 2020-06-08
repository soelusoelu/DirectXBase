#pragma once

#include "ShaderResourceViewDesc.h"
#include "SystemInclude.h"
#include <memory>

class Texture2D;

class ShaderResourceView {
public:
    ShaderResourceView(std::shared_ptr<Texture2D> texture2D, const ShaderResourceViewDesc* desc = nullptr);
    ShaderResourceView(ID3D11ShaderResourceView* view);
    ~ShaderResourceView();
    void setVSShaderResources(unsigned start = 0, unsigned numViews = 1) const;
    void setPSShaderResources(unsigned start = 0, unsigned numViews = 1) const;

private:
    D3D11_SHADER_RESOURCE_VIEW_DESC toSRVDesc(const ShaderResourceViewDesc* desc) const;
    D3D11_SRV_DIMENSION toDimension(SRVDimension dimension) const;

private:
    ID3D11ShaderResourceView* mShaderResourceView;
};
