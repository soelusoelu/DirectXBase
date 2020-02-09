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
class GBuffer;
class InputElement;
class InputElementDesc;
class Mesh;
class Shader;
class SoundBase;
class Sound;
class SubResourceDesc;
class Texture;
class Transform3D;
class VertexStreamDesc;
class ViewportDesc;

class Renderer : public std::enable_shared_from_this<Renderer> {
public:
    Renderer(const HWND& hWnd);
    ~Renderer();
    void initialize();

    ID3D11Device* device() const;
    ID3D11DeviceContext* deviceContext() const;
    ID3D11RasterizerState* rasterizerState() const;
    ID3D11RasterizerState* rasterizerStateBack() const;
    std::shared_ptr<GBuffer> getGBuffer() const;

    Buffer* createRawBuffer(const BufferDesc& desc, const SubResourceDesc* data = nullptr) const;
    std::shared_ptr<Buffer> createBuffer(const BufferDesc& desc, const SubResourceDesc* data = nullptr) const;
    void setViewport(const ViewportDesc& desc);
    void setVertexBuffer(const VertexStreamDesc* stream, unsigned numStream = 1, unsigned start = 0);
    void setIndexBuffer(Buffer* buffer, unsigned offset = 0);
    void setIndexBuffer(std::shared_ptr<Buffer> buffer, unsigned offset = 0);
    void setPrimitive(PrimitiveType primitive);
    void setRenderTargets(ID3D11RenderTargetView* targets[], unsigned numTargets);
    void setDefaultRenderTarget();

    std::shared_ptr<Shader> createShader(const char* fileName);
    std::shared_ptr<Texture> createTexture(const char* fileName);
    std::shared_ptr<Sound> createSound(const char* fileName);
    std::shared_ptr<Sound> createSE(const char* fileName);
    std::shared_ptr<Mesh> createMesh(const char* fileName);

    void draw(unsigned numVertex, unsigned start = 0);
    void drawIndexed(unsigned numIndices, unsigned startIndex = 0, int startVertex = 0);
    void clear();
    void clearRenderTarget(ID3D11RenderTargetView* target);
    void clearDepthStencilView();
    void present();
    ID3D11DepthStencilView* dsv() const { return mDepthStencilView; }

private:
    void createDeviceAndSwapChain(const HWND& hWnd);
    void createRenderTargetView();
    void createDepthStencilView();
    void createDepthStencilState();
    void createRasterizerState();
    void createBlendState();
    D3D11_PRIMITIVE_TOPOLOGY toPrimitiveMode(PrimitiveType primitive);

private:
    ID3D11Device* mDevice;
    ID3D11DeviceContext* mDeviceContext;
    IDXGISwapChain* mSwapChain;
    ID3D11RenderTargetView* mRenderTargetView;
    ID3D11Texture2D* mDepthStencil;
    ID3D11DepthStencilView* mDepthStencilView;
    ID3D11RasterizerState* mRasterizerState;
    ID3D11RasterizerState* mRasterizerStateBack;
    ID3D11DepthStencilState* mDepthStencilState;
    ID3D11BlendState* mBlendState;

    std::unique_ptr<SoundBase> mSoundBase;
    std::shared_ptr<GBuffer> mGBuffer;

    std::unordered_map<const char*, std::shared_ptr<Shader>> mShaders;
    std::unordered_map<const char*, std::shared_ptr<Texture>> mTextures;
    std::unordered_map<const char*, std::shared_ptr<Sound>> mSounds;
    std::unordered_map<const char*, std::shared_ptr<Mesh>> mMeshes;
};

