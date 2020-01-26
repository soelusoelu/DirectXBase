#include "Shader.h"
#include "../Device/Renderer.h"
#include "../Sprite/Texture.h"
#include "../System/Buffer.h"
#include "../System/BufferDesc.h"
#include "../System/Game.h"
#include "../System/InputElement.h"
#include "../System/InputElementDesc.h"

Shader::Shader(std::shared_ptr<Renderer> renderer, const char* fileName) :
    mRenderer(renderer),
    mCompileShader(nullptr),
    mVertexShader(nullptr),
    mPixelShader(nullptr),
    mConstantBuffers(0) {

    createVertexShader(fileName);
    createPixelShader(fileName);
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
    mRenderer->deviceContext()->VSSetShader(mVertexShader, &classInstances, numClassInstances);
}

void Shader::setPSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) {
    mRenderer->deviceContext()->PSSetShader(mPixelShader, &classInstances, numClassInstances);
}

void Shader::createConstantBuffer(unsigned bufferSize, unsigned index) {
    auto num = mConstantBuffers.size();
    if (index >= num) {
        mConstantBuffers.resize(num + 1);
    }

    BufferDesc cb;
    cb.size = bufferSize;
    cb.usage = BufferUsage::BUFFER_USAGE_DYNAMIC;
    cb.type = BufferType::BUFFER_TYPE_CONSTANT_BUFFER;
    cb.cpuAccessFlags = BufferCPUAccessFlag::CPU_ACCESS_WRITE;
    mConstantBuffers[index] = mRenderer->createBuffer(cb);
}

void Shader::setVSConstantBuffers(unsigned start, unsigned numBuffers) {
    auto buf = mConstantBuffers[start]->buffer();
    mRenderer->deviceContext()->VSSetConstantBuffers(start, numBuffers, &buf);
}

void Shader::setPSConstantBuffers(unsigned start, unsigned numBuffers) {
    auto buf = mConstantBuffers[start]->buffer();
    mRenderer->deviceContext()->PSSetConstantBuffers(start, numBuffers, &buf);
}

ID3D11VertexShader* Shader::getVertexShader() const {
    return mVertexShader;
}

ID3D11PixelShader* Shader::getPixelShader() const {
    return mPixelShader;
}

void Shader::createInputLayout(const InputElementDesc* layout, unsigned numElements) {
    mVertexLayout = std::make_shared<InputElement>(mRenderer->device(), layout, numElements, mCompileShader);
}

void Shader::setInputLayout() {
    mRenderer->deviceContext()->IASetInputLayout(mVertexLayout->layout());
}

std::shared_ptr<Buffer> Shader::getConstantBuffer(unsigned index) const {
    return mConstantBuffers[index];
}

void Shader::createVertexShader(const char* fileName) {
    setShaderDirectory();
    //ブロブからバーテックスシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName, nullptr, nullptr, "VS", "vs_5_0", 0, 0, nullptr, &mCompileShader, nullptr, nullptr))) {
        MessageBox(0, L"hlsl読み込み失敗", nullptr, MB_OK);
        return;
    }
    if (FAILED(mRenderer->device()->CreateVertexShader(mCompileShader->GetBufferPointer(), mCompileShader->GetBufferSize(), nullptr, &mVertexShader))) {
        SAFE_RELEASE(mCompileShader);
        MessageBox(0, L"バーテックスシェーダー作成失敗", nullptr, MB_OK);
        return;
    }
}

void Shader::createPixelShader(const char* fileName) {
    ID3D10Blob* compiledShader;
    setShaderDirectory();
    //ブロブからピクセルシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName, nullptr, nullptr, "PS", "ps_5_0", 0, 0, nullptr, &compiledShader, nullptr, nullptr))) {
        MessageBox(0, L"hlsl読み込み失敗", nullptr, MB_OK);
        return;
    }
    if (FAILED(mRenderer->device()->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &mPixelShader))) {
        SAFE_RELEASE(compiledShader);
        MessageBox(0, L"ピクセルシェーダー作成失敗", nullptr, MB_OK);
        return;
    }
    SAFE_RELEASE(compiledShader);
}
