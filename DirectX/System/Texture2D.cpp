#include "Texture2D.h"
#include "Format.h"
#include "Game.h"
#include "Usage.h"
#include "../Device/Renderer.h"

Texture2D::Texture2D(ID3D11Device* device, const Texture2DDesc& desc, const SubResourceDesc* data) :
    mTexture2D(nullptr),
    mDesc(desc) {
    if (data) {
        device->CreateTexture2D(&toTexture2DDesc(desc), &toSubResource(data), &mTexture2D);
    } else {
        device->CreateTexture2D(&toTexture2DDesc(desc), nullptr, &mTexture2D);
    }
}

Texture2D::~Texture2D() {
    SAFE_RELEASE(mTexture2D);
}

ID3D11Texture2D* Texture2D::texture2D() const {
    return mTexture2D;
}

const Texture2DDesc& Texture2D::desc() const {
    return mDesc;
}

D3D11_TEXTURE2D_DESC Texture2D::toTexture2DDesc(const Texture2DDesc& desc) const {
    D3D11_TEXTURE2D_DESC td;
    td.Width = desc.width;
    td.Height = desc.height;
    td.MipLevels = desc.mipLevels;
    td.ArraySize = desc.arraySize;
    td.Format = toFormat(desc.format);
    td.SampleDesc.Count = desc.sampleDesc.count;
    td.SampleDesc.Quality = desc.sampleDesc.quality;
    td.Usage = toUsage(desc.usage);
    td.BindFlags = desc.bindFlags;
    td.CPUAccessFlags = desc.cpuAccessFlags;
    td.MiscFlags = desc.miscFlags;

    return td;
}
