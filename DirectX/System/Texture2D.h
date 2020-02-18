#pragma once

#include "DirectXIncLib.h"
#include "SubResourceDesc.h"
#include "Texture2DDesc.h"
#include <memory>

class Renderer;

class Texture2D {
public:
    Texture2D(ID3D11Device* device, const Texture2DDesc& desc, const SubResourceDesc* data);
    Texture2D(ID3D11Texture2D* texture2D);
    ~Texture2D();
    ID3D11Texture2D* texture2D() const;
    const Texture2DDesc& desc() const;

private:
    D3D11_TEXTURE2D_DESC toTexture2DDesc(const Texture2DDesc& desc) const;

private:
    ID3D11Texture2D* mTexture2D;
    Texture2DDesc mDesc;
};
