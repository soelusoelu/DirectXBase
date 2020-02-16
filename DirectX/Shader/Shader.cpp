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
    mRenderer(renderer),
    mCompileShader(nullptr),
    mVertexShader(nullptr),
    mPixelShader(nullptr),
    mVertexLayout(nullptr) {

    createVertexShader(renderer, fileName);
    createPixelShader(renderer, fileName);
}

Shader::~Shader() {
    SAFE_RELEASE(mCompileShader);
    SAFE_RELEASE(mVertexShader);
    SAFE_RELEASE(mPixelShader);
}

bool Shader::map(MappedSubResourceDesc* data, unsigned index, unsigned sub, D3D11_MAP type, unsigned flag) {
    if (auto r = mRenderer.lock()) {
        auto msr = toMappedSubResource(data);
        auto res = r->deviceContext()->Map(mConstantBuffers[index]->buffer(), sub, type, flag, &msr);

        data->data = msr.pData;
        data->rowPitch = msr.RowPitch;
        data->depthPitch = msr.DepthPitch;

        return (SUCCEEDED(res));
    }
    return false;
}

void Shader::unmap(unsigned index, unsigned sub) {
    if (auto r = mRenderer.lock()) {
        r->deviceContext()->Unmap(mConstantBuffers[index]->buffer(), sub);
    }
}

void Shader::setVSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) {
    if (auto r = mRenderer.lock()) {
        r->deviceContext()->VSSetShader(mVertexShader, &classInstances, numClassInstances);
    }
}

void Shader::setPSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) {
    if (auto r = mRenderer.lock()) {
        r->deviceContext()->PSSetShader(mPixelShader, &classInstances, numClassInstances);
    }
}

void Shader::createConstantBuffer(unsigned bufferSize, unsigned index) {
    auto num = mConstantBuffers.size();
    if (index >= num) {
        mConstantBuffers.resize(num + 1);
    }

    BufferDesc cb;
    cb.size = bufferSize;
    cb.usage = Usage::USAGE_DYNAMIC;
    cb.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_CONSTANT_BUFFER);
    cb.cpuAccessFlags = static_cast<unsigned>(BufferCPUAccessFlag::CPU_ACCESS_WRITE);
    if (auto r = mRenderer.lock()) {
        mConstantBuffers[index] = r->createBuffer(cb);
    }
}

void Shader::setVSConstantBuffers(unsigned index, unsigned numBuffers) {
    if (auto r = mRenderer.lock()) {
        auto buf = mConstantBuffers[index]->buffer();
        r->deviceContext()->VSSetConstantBuffers(index, numBuffers, &buf);
    }
}

void Shader::setPSConstantBuffers(unsigned index, unsigned numBuffers) {
    if (auto r = mRenderer.lock()) {
        auto buf = mConstantBuffers[index]->buffer();
        r->deviceContext()->PSSetConstantBuffers(index, numBuffers, &buf);
    }
}

void Shader::createInputLayout(const InputElementDesc layout[], unsigned numElements) {
    if (auto r = mRenderer.lock()) {
        mVertexLayout = std::make_shared<InputElement>(r->device(), layout, numElements, mCompileShader);
    }
}

void Shader::setInputLayout() {
    if (auto r = mRenderer.lock()) {
        r->deviceContext()->IASetInputLayout(mVertexLayout->layout());
    }
}

std::shared_ptr<Buffer> Shader::getConstantBuffer(unsigned index) const {
    return mConstantBuffers[index];
}

void Shader::createVertexShader(std::shared_ptr<Renderer> renderer, const std::string& fileName) {
    setShaderDirectory();
    //ブロブからバーテックスシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName.c_str(), nullptr, nullptr, "VS", "vs_5_0", 0, 0, nullptr, &mCompileShader, nullptr, nullptr))) {
        MessageBox(0, L"hlsl読み込み失敗", nullptr, MB_OK);
        return;
    }
    if (FAILED(renderer->device()->CreateVertexShader(mCompileShader->GetBufferPointer(), mCompileShader->GetBufferSize(), nullptr, &mVertexShader))) {
        SAFE_RELEASE(mCompileShader);
        MessageBox(0, L"バーテックスシェーダー作成失敗", nullptr, MB_OK);
        return;
    }
}

void Shader::createPixelShader(std::shared_ptr<Renderer> renderer, const std::string& fileName) {
    ID3D10Blob* compiledShader;
    setShaderDirectory();
    //ブロブからピクセルシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName.c_str(), nullptr, nullptr, "PS", "ps_5_0", 0, 0, nullptr, &compiledShader, nullptr, nullptr))) {
        MessageBox(0, L"hlsl読み込み失敗", nullptr, MB_OK);
        return;
    }
    if (FAILED(renderer->device()->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &mPixelShader))) {
        SAFE_RELEASE(compiledShader);
        MessageBox(0, L"ピクセルシェーダー作成失敗", nullptr, MB_OK);
        return;
    }
    SAFE_RELEASE(compiledShader);
}

D3D11_MAPPED_SUBRESOURCE Shader::toMappedSubResource(const MappedSubResourceDesc* desc) const {
    D3D11_MAPPED_SUBRESOURCE msr;
    msr.pData = desc->data;
    msr.RowPitch = desc->rowPitch;
    msr.DepthPitch = desc->depthPitch;

    return msr;
}
