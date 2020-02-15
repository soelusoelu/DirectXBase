﻿#pragma once

#include "DirectXIncLib.h"

//テクスチャの種類
enum class TextureType {
    TEXTURE_TYPE_1D,
    TEXTURE_TYPE_2D,
    TEXTURE_TYPE_3D,
    TEXTURE_TYPE_CUBE
};

//テクスチャフォーマット
enum class TextureFormat {
    TEXTURE_FORMAT_RGBA32_FLOAT,
    TEXTURE_FORMAT_RGBA8_UNORM
};

//テクスチャの使用方法
enum class TextureUsage {
    TEXTURE_USAGE_DEFAULT, //GPUによる読み書き
    TEXTURE_USAGE_IMMUTABLE, //GPUの読み込みのみ可能
    TEXTURE_USAGE_DYNAMIC, //GPUの読み込みとCPUの書き込みが可能
    TEXTURE_USAGE_STAGING //GPUからCPUへのデータ転送をサポート
};

//リソースの使用方法
enum class TextureBind {
    TEXTURE_BIND_SHADER_RESOURCE = 0x8L,
    TEXTURE_BIND_RENDER_TARGET = 0x20L,
    TEXTURE_BIND_DEPTH_STENCIL = 0x40L
};

//cpuアクセス権限
enum class TextureCPUAccessFlag {
    CPU_ACCESS_WRITE = 0x10000L,
    CPU_ACCESS_READ = 0x20000L
};

//テクスチャ補間
enum class TextureFilter {
    TEXTURE_FILTER_POINT, //補間なし
    TEXTURE_FILTER_LINEAR, //線形補間
    TEXTURE_FILTER_TRIANGLE, //高性能、重い
};

struct TextureDesc {
    //幅
    unsigned width;
    //高さ
    unsigned height;
    //深度(2Dテクスチャでは0)
    unsigned depth;
    //ミップマップの最初のレベル 基本0
    unsigned firstMipLevel;
    //ミップマップ数
    unsigned mipLevels;
    //テクスチャの使用方法
    TextureUsage usage;
    //リソースの使用方法
    unsigned bindFlags;
    //cpuアクセス権限
    unsigned cpuAccessFlags;
    //オプション 基本0
    unsigned miscFlags;
    //フォーマット
    TextureFormat format;
    //テクスチャのフィルタリング
    TextureFilter filter;
    //ミップマップのフィルタリング
    TextureFilter mipFilter;
    //読み込む元画像情報
    D3DX11_IMAGE_INFO* srcInfo;

    TextureDesc() :
        width(1),
        height(1),
        depth(0),
        firstMipLevel(0),
        mipLevels(1),
        usage(TextureUsage::TEXTURE_USAGE_DEFAULT),
        bindFlags(static_cast<unsigned>(TextureBind::TEXTURE_BIND_SHADER_RESOURCE)),
        cpuAccessFlags(0),
        miscFlags(0),
        format(TextureFormat::TEXTURE_FORMAT_RGBA8_UNORM),
        filter(TextureFilter::TEXTURE_FILTER_LINEAR),
        mipFilter(TextureFilter::TEXTURE_FILTER_LINEAR),
        srcInfo(0) {
    }
};