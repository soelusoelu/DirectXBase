#pragma once

//バッファタイプ
enum class BufferType {
    BUFFER_TYPE_VERTEX = 0x1L, //頂点バッファ
    BUFFER_TYPE_INDEX = 0x2L, //インデックスバッファ
    BUFFER_TYPE_CONSTANT_BUFFER = 0x4L,
};

enum class BufferUsage {
    BUFFER_USAGE_DEFAULT, //GPUによる読み書き
    BUFFER_USAGE_IMMUTABLE, //GPUの読み込みのみ可能
    BUFFER_USAGE_DYNAMIC, //GPUの読み込みとCPUの書き込みが可能
    BUFFER_USAGE_STAGING //GPUからCPUへのデータ転送をサポート
};

enum class BufferCPUAccessFlag {
    CPU_ACCESS_WRITE = 0x10000L,
    CPU_ACCESS_READ = 0x20000L
};

struct BufferDesc {
    //データのバイト数
    unsigned size;
    //バッファの使用方法
    BufferUsage usage;
    //バッファタイプ(BindFlags)
    unsigned type;
    //cpuアクセス権限
    unsigned cpuAccessFlags;
    //オプション 基本0
    unsigned miscFlags;
    //コンピュートシェーダを使うなら 基本0
    unsigned structureByteStride;

    //コンストラクタ
    BufferDesc() :
        size(0),
        usage(BufferUsage::BUFFER_USAGE_DEFAULT),
        type(static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX)),
        cpuAccessFlags(0),
        miscFlags(0),
        structureByteStride(0) {
    }
};
