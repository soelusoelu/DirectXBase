#include "Shader.h"
#include "../Device/Renderer.h"
#include "../Sprite/Texture.h"
#include "../System/Buffer.h"
#include "../System/BufferDesc.h"
#include "../System/Game.h"
#include "../System/InputElement.h"
#include "../System/InputElementDesc.h"

Shader::Shader(std::shared_ptr<Renderer> renderer, const char* fileName, const char* VSFuncName, const char* PSFuncName) :
    mDevice(renderer->device()),
    mDeviceContext(renderer->deviceContext()),
    mCompileShader(nullptr),
    mVertexShader(nullptr),
    mPixelShader(nullptr),
    mConstantBuffer(nullptr) {
    BufferDesc cb;
    cb.size = sizeof(TextureShaderConstantBuffer);
    cb.usage = BufferUsage::BUFFER_USAGE_DYNAMIC;
    cb.type = BufferType::BUFFER_TYPE_CONSTANT_BUFFER;
    cb.cpuAccessFlags = BufferCPUAccessFlag::CPU_ACCESS_WRITE;
    mConstantBuffer = renderer->createBuffer(cb);

    createVertexShader(fileName, VSFuncName);
    createPixelShader(fileName, PSFuncName);
}

Shader::~Shader() {
    SAFE_RELEASE(mCompileShader);
    SAFE_RELEASE(mVertexShader);
    SAFE_RELEASE(mPixelShader);
}

void Shader::setVertexShader(ID3D11VertexShader* vertex) {
    mVertexShader = vertex;
}

void Shader::setPixelShader(ID3D11PixelShader* pixel) {
    mPixelShader = pixel;
}

void Shader::setVSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) {
    mDeviceContext->VSSetShader(mVertexShader, &classInstances, numClassInstances);
}

void Shader::setPSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) {
    mDeviceContext->PSSetShader(mPixelShader, &classInstances, numClassInstances);
}

void Shader::setVSConstantBuffers(unsigned start, unsigned numBuffers) {
    auto buf = mConstantBuffer->buffer();
    mDeviceContext->VSSetConstantBuffers(start, numBuffers, &buf);
}

void Shader::setPSConstantBuffers(unsigned start, unsigned numBuffers) {
    auto buf = mConstantBuffer->buffer();
    mDeviceContext->PSSetConstantBuffers(start, numBuffers, &buf);
}

void Shader::setVSTextures(std::shared_ptr<Texture> texture, unsigned start, unsigned numTextures) {
    auto tex = texture->texture();
    mDeviceContext->VSSetShaderResources(start, numTextures, &tex);
}

void Shader::setPSTextures(std::shared_ptr<Texture> texture, unsigned start, unsigned numTextures) {
    auto tex = texture->texture();
    mDeviceContext->PSSetShaderResources(start, numTextures, &tex);
}

ID3D11VertexShader* Shader::getVertexShader() const {
    return mVertexShader;
}

ID3D11PixelShader* Shader::getPixelShader() const {
    return mPixelShader;
}

ID3D10Blob* Shader::getCompiledShader() const {
    return mCompileShader;
}

std::shared_ptr<Buffer> Shader::getConstantBuffer() const {
    return mConstantBuffer;
}

void Shader::createVertexShader(const char* fileName, const char* funcName) {
    setShaderDirectory();
    //ブロブからバーテックスシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName, nullptr, nullptr, funcName, "vs_5_0", 0, 0, nullptr, &mCompileShader, nullptr, nullptr))) {
        MessageBox(0, L"hlsl読み込み失敗", nullptr, MB_OK);
        return;
    }
    if (FAILED(mDevice->CreateVertexShader(mCompileShader->GetBufferPointer(), mCompileShader->GetBufferSize(), nullptr, &mVertexShader))) {
        SAFE_RELEASE(mCompileShader);
        MessageBox(0, L"バーテックスシェーダー作成失敗", nullptr, MB_OK);
        return;
    }
}

void Shader::createPixelShader(const char* fileName, const char* funcName) {
    ID3D10Blob* compiledShader;
    setShaderDirectory();
    //ブロブからピクセルシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName, nullptr, nullptr, funcName, "ps_5_0", 0, 0, nullptr, &compiledShader, nullptr, nullptr))) {
        MessageBox(0, L"hlsl読み込み失敗", nullptr, MB_OK);
        return;
    }
    if (FAILED(mDevice->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &mPixelShader))) {
        SAFE_RELEASE(compiledShader);
        MessageBox(0, L"ピクセルシェーダー作成失敗", nullptr, MB_OK);
        return;
    }
    SAFE_RELEASE(compiledShader);
}
