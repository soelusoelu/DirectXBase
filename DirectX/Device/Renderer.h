#pragma once

#include "../Utility/Math.h"
#include "../System/DirectXIncLib.h"
#include <memory>
#include <unordered_map>

enum class PrimitiveType {
    PRIMITIVE_TYPE_POINT_LIST,
    PRIMITIVE_TYPE_LINE_LIST,
    PRIMITIVE_TYPE_LINE_STRIP,
    PRIMITIVE_TYPE_TRIANGLE_LIST,
    PRIMITIVE_TYPE_TRIANGLE_STRIP
};

class Buffer;
class BufferDesc;
class InputElement;
class InputElementDesc;
class Shader;
class SubResourceDesc;
class Texture;
class VertexStreamDesc;
class SoundBase;
class Sound;

class Renderer : public std::enable_shared_from_this<Renderer> {
public:
    Renderer(ID3D11Device* device, ID3D11DeviceContext* context);
    ~Renderer();

    ID3D11Device* device() const;
    ID3D11DeviceContext* deviceContext() const;

    Buffer* createRawBuffer(const BufferDesc& desc, const SubResourceDesc* data = nullptr) const;
    std::shared_ptr<Buffer> createBuffer(const BufferDesc& desc, const SubResourceDesc* data = nullptr) const;
    std::shared_ptr<InputElement> createInputLayout(const InputElementDesc* layout, unsigned numElements, ID3D10Blob* compile) const;
    void setVertexBuffer(const VertexStreamDesc* stream, unsigned numStream = 1, unsigned start = 0);
    void setIndexBuffer(Buffer* buffer, unsigned offset = 0);
    void setInputLayout(std::shared_ptr<InputElement> layout);
    void setPrimitive(PrimitiveType primitive);

    std::shared_ptr<Shader> createShader(const char* fileName);
    std::shared_ptr<Texture> createTexture(const char* fileName);
    std::shared_ptr<Sound> createSound(const char* fileName);
    std::shared_ptr<Sound> createSE(const char* fileName);

    void draw(unsigned numVertex, unsigned start = 0);
    void drawIndexed(unsigned numIndices, unsigned startIndex = 0, int startVertex = 0);
    void clear();

private:
    D3D11_PRIMITIVE_TOPOLOGY toPrimitiveMode(PrimitiveType primitive);

private:
    ID3D11Device* mDevice;
    ID3D11DeviceContext* mDeviceContext;
    std::unique_ptr<SoundBase> mSoundBase;

    std::unordered_map<const char*, std::shared_ptr<Shader>> mShaders;
    std::unordered_map<const char*, std::shared_ptr<Texture>> mTextures;
    std::unordered_map<const char*, std::shared_ptr<Sound>> mSounds;
};

