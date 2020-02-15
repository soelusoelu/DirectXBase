#pragma once

#include "DirectXIncLib.h"

enum class SRVDimension {
    SRV_DIMENSION_TEXTURE2D
};

struct Texture2DShaderResourceView {
    unsigned mostDetailedMip;
    unsigned mipLevels;

    Texture2DShaderResourceView() :
        mostDetailedMip(0),
        mipLevels(1) {
    }
};

struct ShaderResourceViewDesc {
    DXGI_FORMAT format;
    SRVDimension viewDimension;
    Texture2DShaderResourceView texture2D;

    ShaderResourceViewDesc() :
        format(DXGI_FORMAT_R8G8B8A8_UNORM),
        viewDimension(SRVDimension::SRV_DIMENSION_TEXTURE2D),
        texture2D() {
    }
};
