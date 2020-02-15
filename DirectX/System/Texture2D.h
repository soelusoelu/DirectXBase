#pragma once

#include "DirectXIncLib.h"
#include "SubResourceDesc.h"
#include "Texture2DDesc.h"
#include <memory>

class Renderer;

class Texture2D {
public:
    Texture2D(std::shared_ptr<Renderer> renderer);
    ~Texture2D();
    ID3D11Texture2D* createTexture2D(const Texture2DDesc& desc, const SubResourceDesc* data = nullptr) const;

private:
    D3D11_TEXTURE2D_DESC toTexture2DDesc(const Texture2DDesc& desc) const;

private:
    std::weak_ptr<Renderer> mRenderer;
};
