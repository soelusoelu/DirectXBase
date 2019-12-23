#pragma once

#include "BufferDesc.h"
#include "DirectXIncLib.h"
#include "../Device/Renderer.h"

class SubResourceDesc;

class Buffer {
public:
    Buffer(ID3D11Device* device, const BufferDesc& desc, const SubResourceDesc* data);
    virtual ~Buffer();
    //�f�B�X�N���v�^�̎擾
    const BufferDesc& desc() const;
    //�f�[�^�̐ݒ�
    void setData(const void* data);
    //�f�[�^�̎擾
    void getData(void* data) const;
    //�o�b�t�@�̎擾
    ID3D11Buffer* buffer() const;

private:
    //����o�b�t�@�[�f�X�N���v�^��ϊ�
    D3D11_BUFFER_DESC toBufferDesc(const BufferDesc& desc) const;
    //�o�b�t�@�̃^�C�v��ϊ�
    unsigned toTarget(BufferType type) const;
    //�o�b�t�@�̎g�p���@��ϊ�
    D3D11_USAGE toUsage(BufferUsage usage) const;
    //CPU�A�N�Z�X������ϊ�
    unsigned toCPUAccess(BufferCPUAccessFlag flag) const;
    //�T�u���\�[�X��ϊ�
    D3D11_SUBRESOURCE_DATA toSubResource(const SubResourceDesc* data) const;

    //�R�s�[�֎~
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

private:
    //�f�B�X�N���v�^
    BufferDesc mDesc;
    //�o�b�t�@�I�u�W�F�N�g
    ID3D11Buffer* mBuffer;
};

