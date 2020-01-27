#include "Renderer.h"
#include "Sound.h"
#include "../Mesh/Mesh.h"
#include "../System/Buffer.h"
#include "../Shader/Shader.h"
#include "../System/DirectXIncLib.h"
#include "../System/Direct3D11.h"
#include "../System/InputElement.h"
#include "../System/SubResourceDesc.h"
#include "../System/VertexStreamDesc.h"
#include "../Sprite/Texture.h"
#include "../Sprite/Sprite.h"

Renderer::Renderer(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11RasterizerState* front, ID3D11RasterizerState* back) :
    mDevice(device),
    mDeviceContext(context),
    mRasterizerState(front),
    mRasterizerStateBack(back),
    mSoundBase(std::make_unique<SoundBase>()) {
}

Renderer::~Renderer() {
    clear();
}

ID3D11Device* Renderer::device() const {
    return mDevice;
}

ID3D11DeviceContext* Renderer::deviceContext() const {
    return mDeviceContext;
}

ID3D11RasterizerState* Renderer::rasterizerState() const {
    return mRasterizerState;
}

ID3D11RasterizerState* Renderer::rasterizerStateBack() const {
    return mRasterizerStateBack;
}

Buffer* Renderer::createRawBuffer(const BufferDesc& desc, const SubResourceDesc* data) const {
    return new Buffer(mDevice, desc, data);
}

std::shared_ptr<Buffer> Renderer::createBuffer(const BufferDesc& desc, const SubResourceDesc* data) const {
    return std::make_shared<Buffer>(mDevice, desc, data);
}

void Renderer::setVertexBuffer(const VertexStreamDesc* stream, unsigned numStream, unsigned start) {
    /* IASetVertexBuffers
        使い始めのスロット番号
        頂点バッファ配列の要素数
        頂点バッファ配列の先頭ポインタ
        INPUT_ELEMENT_DESC構造体のサイズが入った配列への先頭ポインタ(stride(読み込み単位)として扱うため)
        頂点バッファ配列の各頂点バッファの頭出しをするオフセット値の配列
    */
    ID3D11Buffer* buffer;
    if (stream->buffer) {
        buffer = stream->buffer->buffer();
    } else {
        buffer = stream->sharedBuffer->buffer();
    }
    mDeviceContext->IASetVertexBuffers(start, numStream, &buffer, &stream->stride, &stream->offset);
    //buffer->Release();
}

void Renderer::setIndexBuffer(Buffer* buffer, unsigned offset) {
    mDeviceContext->IASetIndexBuffer(buffer->buffer(), DXGI_FORMAT_R16_UINT, offset);
}

void Renderer::setIndexBuffer(std::shared_ptr<Buffer> buffer, unsigned offset) {
    mDeviceContext->IASetIndexBuffer(buffer->buffer(), DXGI_FORMAT_R32_UINT, offset);
}

void Renderer::setPrimitive(PrimitiveType primitive) {
    mDeviceContext->IASetPrimitiveTopology(toPrimitiveMode(primitive));
}

std::shared_ptr<Shader> Renderer::createShader(const char* fileName) {
    std::shared_ptr<Shader> shader;
    auto itr = mShaders.find(fileName);
    if (itr != mShaders.end()) { //既に読み込まれている
        shader = itr->second;
    } else { //初読み込み
        shader = std::make_shared<Shader>(shared_from_this(), fileName);
        mShaders.emplace(fileName, shader);
    }
    return shader;
}

std::shared_ptr<Texture> Renderer::createTexture(const char* fileName) {
    std::shared_ptr<Texture> texture;
    auto itr = mTextures.find(fileName);
    if (itr != mTextures.end()) { //既に読み込まれている
        texture = itr->second;
    } else { //初読み込み
        texture = std::make_shared<Texture>(shared_from_this(), fileName);
        mTextures.emplace(fileName, texture);
    }
    return texture;
}

std::shared_ptr<Sound> Renderer::createSound(const char* fileName) {
    std::shared_ptr<Sound> sound;
    auto itr = mSounds.find(fileName);
    if (itr != mSounds.end()) { //既に読み込まれている
        sound = itr->second;
    } else { //初読み込み
        sound = std::make_shared<Sound>();
        mSoundBase->load(fileName, &sound);
        mSounds.emplace(fileName, sound);
    }
    return sound;
}

std::shared_ptr<Sound> Renderer::createSE(const char* fileName) {
    auto sound = createSound(fileName);
    mSoundBase->createSourceVoice(&sound);
    return sound;
}

std::shared_ptr<Mesh> Renderer::createMesh(const char* fileName) {
    std::shared_ptr<Mesh> mesh;
    auto itr = mMeshes.find(fileName);
    if (itr != mMeshes.end()) { //既に読み込まれている
        mesh = itr->second;
    } else { //初読み込み
        mesh = std::make_shared<Mesh>(shared_from_this(), fileName);
        mMeshes.emplace(fileName, mesh);
    }
    return mesh;
}

void Renderer::draw(unsigned numVertex, unsigned start) {
    mDeviceContext->Draw(numVertex, start);
}

void Renderer::drawIndexed(unsigned numIndices, unsigned startIndex, int startVertex) {
    mDeviceContext->DrawIndexed(numIndices, startIndex, startVertex);
}

void Renderer::clear() {
    mShaders.clear();
    mTextures.clear();
    mSounds.clear();
    mMeshes.clear();
}

D3D11_PRIMITIVE_TOPOLOGY Renderer::toPrimitiveMode(PrimitiveType primitive) {
    static const D3D11_PRIMITIVE_TOPOLOGY primitiveModes[] = {
        D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, //PRIMITIVE_TYPE_POINTLIST = 0
        D3D11_PRIMITIVE_TOPOLOGY_LINELIST, //PRIMITIVE_TYPE_LINELIST = 1
        D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, //PRIMITIVE_TYPE_LINESTRIP = 2
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, //PRIMITIVE_TYPE_TRIANGLE_LIST = 3
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP //PRIMITIVE_TYPE_TRIANGLE_STRIP = 4
    };
    return primitiveModes[static_cast<int>(primitive)];
}
