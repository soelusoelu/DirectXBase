#include "Renderer.h"
#include "../System/Buffer.h"
#include "../Shader/Shader.h"
#include "../System/DirectXIncLib.h"
#include "../System/Direct3D11.h"
#include "../System/InputElement.h"
#include "../System/SubResourceDesc.h"
#include "../System/VertexStreamDesc.h"
#include "../Sprite/Texture.h"
#include "../Sprite/Sprite.h"
#include <string>

Renderer::Renderer(ID3D11Device* device, ID3D11DeviceContext* context) :
    mDevice(device),
    mDeviceContext(context) {
}

Renderer::~Renderer() = default;

ID3D11Device* Renderer::device() const {
    return mDevice;
}

ID3D11DeviceContext* Renderer::deviceContext() const {
    return mDeviceContext;
}

std::shared_ptr<Buffer> Renderer::createBuffer(const BufferDesc& desc, const SubResourceDesc* data) {
    return std::make_shared<Buffer>(mDevice, desc, data);
}

std::shared_ptr<InputElement> Renderer::createInputLayout(const InputElementDesc* layout, unsigned numElements, ID3D10Blob* compile) {
    return std::make_shared<InputElement>(mDevice, layout, numElements, compile);
}

void Renderer::setVertexBuffer(const VertexStreamDesc * stream, unsigned numStream, unsigned start) {
    /* IASetVertexBuffers
        使い始めのスロット番号
        頂点バッファ配列の要素数
        頂点バッファ配列の先頭ポインタ
        INPUT_ELEMENT_DESC構造体のサイズが入った配列への先頭ポインタ(stride(読み込み単位)として扱うため)
        頂点バッファ配列の各頂点バッファの頭出しをするオフセット値の配列
    */
    auto buffer = stream->buffer->buffer();
    mDeviceContext->IASetVertexBuffers(start, numStream, &buffer, &stream->stride, &stream->offset);
}

void Renderer::setIndexBuffer(std::shared_ptr<Buffer> buffer, unsigned offset) {
    mDeviceContext->IASetIndexBuffer(buffer->buffer(), DXGI_FORMAT_R16_UINT, offset);
}

void Renderer::setInputLayout(std::shared_ptr<InputElement> layout) {
    mDeviceContext->IASetInputLayout(layout->layout());
}

void Renderer::setPrimitive(PrimitiveType primitive) {
    mDeviceContext->IASetPrimitiveTopology(toPrimitiveMode(primitive));
}

std::shared_ptr<Shader> Renderer::createShader(const char* fileName, const char* VSFuncName, const char* PSFuncName) {
    std::shared_ptr<Shader> shader;
    auto itr = mShaders.find(fileName);
    if (itr != mShaders.end()) { //既に読み込まれている
        shader = itr->second;
    } else { //初読み込み
        shader = std::make_shared<Shader>(shared_from_this(), fileName, VSFuncName, PSFuncName);
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

void Renderer::draw(unsigned numVertex, unsigned start) {
    mDeviceContext->Draw(numVertex, start);
}

void Renderer::drawIndexed(unsigned numIndices, unsigned startIndex, int startVertex) {
    mDeviceContext->DrawIndexed(numIndices, startIndex, startVertex);
}

//現状重い
void Renderer::drawNumber(int number, const Vector2 & position, bool isRightAlignment) {
    // マイナスの場合は0
    number = Math::Max<int>(number, 0);

    constexpr int widthChar = 32; // 画像横幅
    constexpr int widthOfTexture = 512;
    constexpr float widthRate = static_cast<float>(widthChar) / static_cast<float>(widthOfTexture);

    //桁数計算(本当は=1)
    int digit = 0;
    if (isRightAlignment) {
        for (int i = number; i >= 10; i /= 10) {
            digit++;
        }
    }

    auto sprite = new Sprite(shared_from_this(), "number.png", 0.01f);
    sprite->setPosition(Vector2(position.x - widthChar * digit, position.y));

    // 数字を文字列化し、1文字ずつ取り出す
    for (auto n : std::to_string(number)) {
        // 数字のテクスチャが数字1つにつき幅32高さ64
        // 文字と文字を引き算し、整数値を取得している
        float num = (n - '0') * widthChar;
        num /= widthOfTexture;
        auto a = num + widthRate;
        sprite->setUV(num, 0, a, 1);
        sprite->copy();

        // 1文字描画したら1桁分右にずらす
        sprite->setPosition(Vector2(sprite->getPosition().x + widthChar, sprite->getPosition().y));
    }
    Sprite::destroy(sprite);
}

void Renderer::clear() {
    mShaders.clear();
    mTextures.clear();
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
