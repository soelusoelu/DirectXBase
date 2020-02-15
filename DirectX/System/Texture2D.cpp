#include "Texture2D.h"
#include "Usage.h"
#include "../Device/Renderer.h"

Texture2D::Texture2D(std::shared_ptr<Renderer> renderer) :
    mRenderer(renderer) {
}

Texture2D::~Texture2D() = default;

ID3D11Texture2D* Texture2D::createTexture2D(const Texture2DDesc & desc, SubResourceDesc * data) const {
    if (auto r = mRenderer.lock()) {
        ID3D11Texture2D* texture;

        r->device()->CreateTexture2D(&toTexture2DDesc(desc), toSubResourceData(data), &texture);

        return texture;
    }
    return nullptr;
}

D3D11_TEXTURE2D_DESC Texture2D::toTexture2DDesc(const Texture2DDesc & desc) const {
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

D3D11_SUBRESOURCE_DATA* Texture2D::toSubResourceData(SubResourceDesc * data) const {
    D3D11_SUBRESOURCE_DATA* srd = nullptr;
    if (!data) {
        return srd;
    }
    srd->pSysMem = data->data;
    srd->SysMemPitch = data->pitch;
    srd->SysMemSlicePitch = data->slicePitch;

    return srd;
}
