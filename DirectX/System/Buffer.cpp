#include "Buffer.h"
#include "Game.h"
#include "Usage.h"
#include "../Device/Renderer.h"

Buffer::Buffer(std::shared_ptr<Renderer> renderer, const BufferDesc& desc, const SubResourceDesc* data) :
    mDesc(desc) {
    //バッファの作成
    if (data) {
        renderer->device()->CreateBuffer(&toBufferDesc(desc), &toSubResource(data), &mBuffer);
    } else {
        renderer->device()->CreateBuffer(&toBufferDesc(desc), nullptr, &mBuffer);
    }
}

Buffer::~Buffer() {
    //バッファを削除
    SAFE_RELEASE(mBuffer);
}

const BufferDesc& Buffer::desc() const {
    return mDesc;
}

ID3D11Buffer* Buffer::buffer() const {
    return mBuffer;
}

D3D11_BUFFER_DESC Buffer::toBufferDesc(const BufferDesc& desc) const {
    D3D11_BUFFER_DESC bd;
    bd.ByteWidth = desc.size;
    bd.Usage = toUsage(desc.usage);
    bd.BindFlags = desc.type;
    bd.CPUAccessFlags = desc.cpuAccessFlags;
    bd.MiscFlags = desc.miscFlags;
    bd.StructureByteStride = desc.structureByteStride;

    return bd;
}
