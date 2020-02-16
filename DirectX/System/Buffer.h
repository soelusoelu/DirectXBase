#pragma once

#include "BufferDesc.h"
#include "DirectXIncLib.h"
#include "SubResourceDesc.h"
#include <memory>

class Renderer;

class Buffer {
public:
    Buffer(std::shared_ptr<Renderer> renderer, const BufferDesc& desc, const SubResourceDesc* data);
    virtual ~Buffer();
    const BufferDesc& desc() const;
    ID3D11Buffer* buffer() const;

private:
    D3D11_BUFFER_DESC toBufferDesc(const BufferDesc& desc) const;

    //コピー禁止
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

protected:
    BufferDesc mDesc;
    ID3D11Buffer* mBuffer;
};

