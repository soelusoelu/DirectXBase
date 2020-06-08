#include "ShaderResourceView.h"
#include "DirectX.h"
#include "Format.h"
#include "GlobalFunction.h"
#include "Texture2D.h"

ShaderResourceView::ShaderResourceView(std::shared_ptr<Texture2D> texture2D, const ShaderResourceViewDesc* desc) :
    mShaderResourceView(nullptr) {
    if (desc) {
        Singleton<DirectX>::instance().device()->CreateShaderResourceView(texture2D->texture2D(), &toSRVDesc(desc), &mShaderResourceView);
    } else {
        Singleton<DirectX>::instance().device()->CreateShaderResourceView(texture2D->texture2D(), nullptr, &mShaderResourceView);
    }
}

ShaderResourceView::ShaderResourceView(ID3D11ShaderResourceView* view) :
    mShaderResourceView(view) {
}

ShaderResourceView::~ShaderResourceView() {
    safeRelease<ID3D11ShaderResourceView>(mShaderResourceView);
}

void ShaderResourceView::setVSShaderResources(unsigned start, unsigned numViews) const {
    Singleton<DirectX>::instance().deviceContext()->VSSetShaderResources(start, numViews, &mShaderResourceView);
}

void ShaderResourceView::setPSShaderResources(unsigned start, unsigned numViews) const {
    Singleton<DirectX>::instance().deviceContext()->PSSetShaderResources(start, numViews, &mShaderResourceView);
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
