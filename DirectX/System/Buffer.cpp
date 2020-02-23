#include "Buffer.h"
#include "DirectX.h"
#include "Game.h"
#include "Usage.h"

Buffer::Buffer(const BufferDesc& desc, const SubResourceDesc* data) :
    mDesc(desc) {
    //バッファの作成
    if (data) {
        Singleton<DirectX>::instance().device()->CreateBuffer(&toBufferDesc(desc), &toSubResource(data), &mBuffer);
    } else {
        Singleton<DirectX>::instance().device()->CreateBuffer(&toBufferDesc(desc), nullptr, &mBuffer);
    }
}

Buffer::~Buffer() {
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
