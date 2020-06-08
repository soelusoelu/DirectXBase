#pragma once

#include "Buffer.h"
#include "BufferDesc.h"
#include "SubResourceDesc.h"
#include "Format.h"

class IndexBuffer : public Buffer {
public:
    IndexBuffer(const BufferDesc& desc, const SubResourceDesc* data);
    ~IndexBuffer();
    void setIndexBuffer(Format format = Format::FORMAT_R32_UINT, unsigned offset = 0);
};
