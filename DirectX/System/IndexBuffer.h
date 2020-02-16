#pragma once

#include "Buffer.h"
#include "BufferDesc.h"
#include "SubResourceDesc.h"
#include "Format.h"
#include <memory>

class Renderer;

class IndexBuffer : public Buffer {
public:
    IndexBuffer(std::shared_ptr<Renderer> renderer, const BufferDesc& desc, const SubResourceDesc* data);
    ~IndexBuffer();
    void setIndexBuffer(Format format = Format::FORMAT_R32_UINT, unsigned offset = 0);

private:
    std::weak_ptr<Renderer> mRenderer;
};
