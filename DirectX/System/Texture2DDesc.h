#pragma once

#include "Format.h"

//テクスチャの使用方法
enum class Texture2DUsage {
    TEXTURE_USAGE_DEFAULT, //GPUによる読み書き
    TEXTURE_USAGE_IMMUTABLE, //GPUの読み込みのみ可能
    TEXTURE_USAGE_DYNAMIC, //GPUの読み込みとCPUの書き込みが可能
    TEXTURE_USAGE_STAGING //GPUからCPUへのデータ転送をサポート
};

//リソースの使用方法
enum class Texture2DBind {
    TEXTURE_BIND_SHADER_RESOURCE = 0x8L,
    TEXTURE_BIND_RENDER_TARGET = 0x20L,
    TEXTURE_BIND_DEPTH_STENCIL = 0x40L
};

//cpuアクセス権限
enum class Texture2DCPUAccessFlag {
    CPU_ACCESS_WRITE = 0x10000L,
    CPU_ACCESS_READ = 0x20000L
};

struct TextureSampleDesc {
    //ピクセルごとのマルチサンプルの数
    unsigned count;
    //画質レベル
    unsigned quality;

    TextureSampleDesc() :
        count(1),
        quality(0) {
    }
};

struct Texture2DDesc {
    //幅
    unsigned width;
    //高さ
    unsigned height;
    //ミップマップ数
    unsigned mipLevels;
    //テクスチャ配列内のテクスチャの数
    unsigned arraySize;
    //フォーマット
    Format format;
    //サンプリングパラメータ指定
    TextureSampleDesc sampleDesc;
    //テクスチャの使用方法
    Texture2DUsage usage;
    //リソースの使用方法
    unsigned bindFlags;
    //cpuアクセス権限
    unsigned cpuAccessFlags;
    //オプション 基本0
    unsigned miscFlags;

    Texture2DDesc() :
        width(0),
        height(0),
        mipLevels(1),
        arraySize(1),
        format(Format::FORMAT_RGBA8_UNORM),
        sampleDesc(),
        usage(Texture2DUsage::TEXTURE_USAGE_DEFAULT),
        bindFlags(static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_SHADER_RESOURCE)),
        cpuAccessFlags(0),
        miscFlags(0) {
    }
};