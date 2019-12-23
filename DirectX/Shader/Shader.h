#pragma once

#include "../System/DirectXIncLib.h"
#include <memory>

class Buffer;
class Renderer;
class Texture;

class Shader {
public:
    Shader(std::shared_ptr<Renderer> renderer, const char* fileName, const char* VSFuncName, const char* PSFuncName);
    ~Shader();
    //�V�F�[�_�[���Z�b�g
    void setVertexShader(ID3D11VertexShader* vertex);
    void setPixelShader(ID3D11PixelShader* pixel);
    //���g���V�F�[�_�[�Ƃ��ēo�^
    void setVSShader(ID3D11ClassInstance* classInstances = nullptr, unsigned numClassInstances = 0);
    void setPSShader(ID3D11ClassInstance* classInstances = nullptr, unsigned numClassInstances = 0);
    //�g�p����R���X�^���g�o�b�t�@��o�^
    void setVSConstantBuffers(unsigned start = 0, unsigned numBuffers = 1);
    void setPSConstantBuffers(unsigned start = 0, unsigned numBuffers = 1);
    //�e�N�X�`���̓o�^
    void setVSTextures(std::shared_ptr<Texture> texture, unsigned start = 0, unsigned numTextures = 1);
    void setPSTextures(std::shared_ptr<Texture> texture, unsigned start = 0, unsigned numTextures = 1);
    //�V�F�[�_�̎擾
    ID3D11VertexShader* getVertexShader() const;
    ID3D11PixelShader* getPixelShader() const;
    //�R���p�C���ς݃V�F�[�_�̎擾
    ID3D10Blob* getCompiledShader() const;
    //�R���X�^���g�o�b�t�@�̎擾
    std::shared_ptr<Buffer> getConstantBuffer() const;

private:
    //�V�F�[�_�̐���
    void createVertexShader(const char* fileName, const char* funcName);
    void createPixelShader(const char* fileName, const char* funcName);

private:
    ID3D11Device* mDevice;
    ID3D11DeviceContext* mDeviceContext;
    ID3D10Blob* mCompileShader;
    ID3D11VertexShader* mVertexShader;
    ID3D11PixelShader* mPixelShader;
    std::shared_ptr<Buffer> mConstantBuffer;
};
