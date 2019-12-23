#include "Buffer.h"
#include "Game.h"
#include "SubResourceDesc.h"

Buffer::Buffer(ID3D11Device* device, const BufferDesc& desc, const SubResourceDesc* data) :
    mDesc(desc) {
    //バッファの作成
    if (data) {
        device->CreateBuffer(&toBufferDesc(desc), &toSubResource(data), &mBuffer);
    } else {
        device->CreateBuffer(&toBufferDesc(desc), nullptr, &mBuffer);
    }
}

Buffer::~Buffer() {
    //バッファを削除
    SAFE_RELEASE(mBuffer);
}

const BufferDesc& Buffer::desc() const {
    return mDesc;
}

void Buffer::setData(const void* data) {
    //glBindBufferARB(mTarget, mBuffer);
    //glBufferSubDataARB(mTarget, 0, mDesc.size, data);
    //glBindBufferARB(mTarget, 0);
    //mDeviceContext->UpdateSubresource()
}

void Buffer::getData(void* data) const {
    //glBindBufferARB(mTarget, mBuffer);
    //glGetBufferSubDataARB(mTarget, 0, mDesc.size, data);
    //glBindBufferARB(mTarget, 0);
}

ID3D11Buffer* Buffer::buffer() const {
    return mBuffer;
}

D3D11_BUFFER_DESC Buffer::toBufferDesc(const BufferDesc& desc) const {
    D3D11_BUFFER_DESC bd;
    bd.ByteWidth = desc.size;
    bd.Usage = toUsage(desc.usage);
    bd.BindFlags = toTarget(desc.type);
    bd.CPUAccessFlags = toCPUAccess(desc.cpuAccessFlags);
    bd.MiscFlags = desc.miscFlags;
    bd.StructureByteStride = desc.structureByteStride;

    return bd;
}

unsigned Buffer::toTarget(BufferType type) const {
    const unsigned targets[] = {
        D3D11_BIND_VERTEX_BUFFER,
        D3D11_BIND_INDEX_BUFFER,
        D3D11_BIND_CONSTANT_BUFFER,
        D3D11_BIND_SHADER_RESOURCE,
        D3D11_BIND_STREAM_OUTPUT,
        D3D11_BIND_RENDER_TARGET,
        D3D11_BIND_DEPTH_STENCIL,
        D3D11_BIND_UNORDERED_ACCESS,
        //D3D11_BIND_DECODER,
        //D3D11_BIND_VIDEO_ENCODER
    };
    return targets[static_cast<unsigned>(type)];
}

D3D11_USAGE Buffer::toUsage(BufferUsage usage) const {
    const D3D11_USAGE usages[] = {
        D3D11_USAGE_DEFAULT,
        D3D11_USAGE_IMMUTABLE,
        D3D11_USAGE_DYNAMIC,
        D3D11_USAGE_STAGING
    };
    return usages[static_cast<unsigned>(usage)];
}

unsigned Buffer::toCPUAccess(BufferCPUAccessFlag flag) const {
    const unsigned accesses[] = {
        0,
        D3D11_CPU_ACCESS_WRITE,
        D3D11_CPU_ACCESS_READ
    };
    return accesses[static_cast<unsigned>(flag)];
}

D3D11_SUBRESOURCE_DATA Buffer::toSubResource(const SubResourceDesc* data) const {
    D3D11_SUBRESOURCE_DATA sub;
    sub.pSysMem = data->data;
    sub.SysMemPitch = data->pitch;
    sub.SysMemSlicePitch = data->slicePitch;

    return sub;
}
