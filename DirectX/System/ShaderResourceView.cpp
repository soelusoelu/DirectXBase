#include "ShaderResourceView.h"
#include "Format.h"
#include "Game.h"
#include "Texture2D.h"
#include "../Device/Renderer.h"

ShaderResourceView::ShaderResourceView(std::shared_ptr<Renderer> renderer, std::shared_ptr<Texture2D> texture2D, const ShaderResourceViewDesc* desc) :
    mRenderer(renderer),
    mShaderResourceView(nullptr) {
    if (desc) {
        renderer->device()->CreateShaderResourceView(texture2D->texture2D(), &toSRVDesc(desc), &mShaderResourceView);
    } else {
        renderer->device()->CreateShaderResourceView(texture2D->texture2D(), nullptr, &mShaderResourceView);
    }
}

ShaderResourceView::ShaderResourceView(std::shared_ptr<Renderer> renderer, ID3D11ShaderResourceView* view) :
    mRenderer(renderer),
    mShaderResourceView(view) {
}

ShaderResourceView::~ShaderResourceView() {
    SAFE_RELEASE(mShaderResourceView);
}

void ShaderResourceView::setVSShaderResources(unsigned start, unsigned numViews) {
    if (auto r = mRenderer.lock()) {
        r->deviceContext()->VSSetShaderResources(start, numViews, &mShaderResourceView);
    }
}

void ShaderResourceView::setPSShaderResources(unsigned start, unsigned numViews) {
    if (auto r = mRenderer.lock()) {
        r->deviceContext()->PSSetShaderResources(start, numViews, &mShaderResourceView);
    }
}

D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceView::toSRVDesc(const ShaderResourceViewDesc* desc) const {
    D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
    srvd.Format = toFormat(desc->format);
    srvd.ViewDimension = toDimension(desc->viewDimension);
    srvd.Texture2D.MostDetailedMip = desc->texture2D.mostDetailedMip;
    srvd.Texture2D.MipLevels = desc->texture2D.mipLevels;

    return srvd;
}

D3D11_SRV_DIMENSION ShaderResourceView::toDimension(SRVDimension dimension) const {
    static constexpr D3D11_SRV_DIMENSION dimensions[]{
        D3D11_SRV_DIMENSION_TEXTURE2D
    };

    return dimensions[static_cast<unsigned>(dimension)];
}
