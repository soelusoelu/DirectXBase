#pragma once

#include "BufferDesc.h"
#include "DirectXIncLib.h"
#include "../Device/Renderer.h"

class SubResourceDesc;

class Buffer {
public:
    Buffer(ID3D11Device* device, const BufferDesc& desc, const SubResourceDesc* data);
    virtual ~Buffer();
    //ディスクリプタの取得
    const BufferDesc& desc() const;
    //データの設定
    void setData(const void* data);
    //データの取得
    void getData(void* data) const;
    //バッファの取得
    ID3D11Buffer* buffer() const;

private:
    //自作バッファーデスクリプタを変換
    D3D11_BUFFER_DESC toBufferDesc(const BufferDesc& desc) const;
    //バッファのタイプを変換
    unsigned toTarget(BufferType type) const;
    //バッファの使用方法を変換
    D3D11_USAGE toUsage(BufferUsage usage) const;
    //CPUアクセス権限を変換
    unsigned toCPUAccess(BufferCPUAccessFlag flag) const;
    //サブリソースを変換
    D3D11_SUBRESOURCE_DATA toSubResource(const SubResourceDesc* data) const;

    //コピー禁止
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

private:
    //ディスクリプタ
    BufferDesc mDesc;
    //バッファオブジェクト
    ID3D11Buffer* mBuffer;
};

