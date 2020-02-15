#include "Shader.h"
#include "../Device/Renderer.h"
#include "../System/Buffer.h"
#include "../System/BufferDesc.h"
#include "../System/Game.h"
#include "../System/InputElement.h"
#include "../System/InputElementDesc.h"
#include "../System/Texture.h"
#include "../System/Usage.h"

Shader::Shader(std::shared_ptr<Renderer> renderer, const std::string& fileName) :
    mDevice(renderer->device()),
    mDeviceContext(renderer->deviceContext()),
    mCompileShader(nullptr),
    mVertexShader(nullptr),
    mPixelShader(nullptr),
    mVertexLayout(nullptr) {

    createVertexShader(fileName);
    createPixelShader(fileName);
}

Shader::~Shader() {
    SAFE_RELEASE(mCompileShader);
    SAFE_RELEASE(mVertexShader);
    SAFE_RELEASE(mPixelShader);
}

bool Shader::map(D3D11_MAPPED_SUBRESOURCE* data, unsigned index, unsigned sub, D3D11_MAP type, unsigned flag) {
    auto res = mDeviceContext->Map(mConstantBuffers[index]->buffer(), sub, type, flag, data);
    return (SUCCEEDED(res));
}

void Shader::unmap(unsigned index, unsigned sub) {
    mDeviceContext->Unmap(mConstantBuffers[index]->buffer(), sub);
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

void Shader::createConstantBuffer(std::shared_ptr<Renderer> renderer, unsigned bufferSize, unsigned index) {
    auto num = mConstantBuffers.size();
    if (index >= num) {
        mConstantBuffers.resize(num + 1);
    }

    BufferDesc cb;
    cb.size = bufferSize;
    cb.usage = Usage::USAGE_DYNAMIC;
    cb.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_CONSTANT_BUFFER);
    cb.cpuAccessFlags = static_cast<unsigned>(BufferCPUAccessFlag::CPU_ACCESS_WRITE);
    mConstantBuffers[index] = renderer->createBuffer(cb);
}

void Shader::setVSConstantBuffers(unsigned index, unsigned numBuffers) {
    auto buf = mConstantBuffers[index]->buffer();
    mDeviceContext->VSSetConstantBuffers(index, numBuffers, &buf);
}

void Shader::setPSConstantBuffers(unsigned index, unsigned numBuffers) {
    auto buf = mConstantBuffers[index]->buffer();
    mDeviceContext->PSSetConstantBuffers(index, numBuffers, &buf);
}

ID3D11VertexShader* Shader::getVertexShader() const {
    return mVertexShader;
}

ID3D11PixelShader* Shader::getPixelShader() const {
    return mPixelShader;
}

void Shader::createInputLayout(const InputElementDesc layout[], unsigned numElements) {
    mVertexLayout = std::make_shared<InputElement>(mDevice, layout, numElements, mCompileShader);
}

void Shader::setInputLayout() {
    mDeviceContext->IASetInputLayout(mVertexLayout->layout());
}

std::shared_ptr<Buffer> Shader::getConstantBuffer(unsigned index) const {
    return mConstantBuffers[index];
}

void Shader::createVertexShader(const std::string& fileName) {
    setShaderDirectory();
    //ブロブからバーテックスシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName.c_str(), nullptr, nullptr, "VS", "vs_5_0", 0, 0, nullptr, &mCompileShader, nullptr, nullptr))) {
        MessageBox(0, L"hlsl読み込み失敗", nullptr, MB_OK);
        return;
    }
    if (FAILED(mDevice->CreateVertexShader(mCompileShader->GetBufferPointer(), mCompileShader->GetBufferSize(), nullptr, &mVertexShader))) {
        SAFE_RELEASE(mCompileShader);
        MessageBox(0, L"バーテックスシェーダー作成失敗", nullptr, MB_OK);
        return;
    }
}

void Shader::createPixelShader(const std::string& fileName) {
    ID3D10Blob* compiledShader;
    setShaderDirectory();
    //ブロブからピクセルシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName.c_str(), nullptr, nullptr, "PS", "ps_5_0", 0, 0, nullptr, &compiledShader, nullptr, nullptr))) {
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
