#pragma once

//バッファタイプ
enum class BufferType {
    BUFFER_TYPE_VERTEX, //頂点バッファ
    BUFFER_TYPE_INDEX, //インデックスバッファ
    BUFFER_TYPE_CONSTANT_BUFFER,
    BUFFER_TYPE_SHADER_RESOURCE,
    BUFFER_TYPE_STREAM_OUTPUT,
    BUFFER_TYPE_RENDER_TARGET,
    BUFFER_TYPE_DEPTH_STENCIL,
    BUFFER_TYPE_UNORDERED_ACCESS,
    //BUFFER_TYPE_DECODER,
    //BUFFER_TYPE_VIDEO_ENCODER
};

enum class BufferUsage {
    BUFFER_USAGE_DEFAULT, //GPUによる読み書き
    BUFFER_USAGE_IMMUTABLE, //GPUの読み込みのみ可能
    BUFFER_USAGE_DYNAMIC, //GPUの読み込みとCPUの書き込みが可能
    BUFFER_USAGE_STAGING //GPUからCPUへのデータ転送をサポート
};

enum class BufferCPUAccessFlag {
    CPU_ACCESS_NONE,
    CPU_ACCESS_WRITE,
    CPU_ACCESS_READ
};

struct BufferDesc {
    //データのバイト数
    unsigned size;
    //バッファの使用方法
    BufferUsage usage;
    //バッファタイプ(BindFlags)
    BufferType type;
    //cpuアクセス権限
    BufferCPUAccessFlag cpuAccessFlags;
    //オプション 基本0
    unsigned miscFlags;
    //コンピュートシェーダを使うなら 基本0
    unsigned structureByteStride;

    //コンストラクタ
    BufferDesc() :
        size(0),
        usage(BufferUsage::BUFFER_USAGE_DEFAULT),
        type(BufferType::BUFFER_TYPE_VERTEX),
        cpuAccessFlags(BufferCPUAccessFlag::CPU_ACCESS_NONE),
        miscFlags(0),
        structureByteStride(0) {
    }
};
