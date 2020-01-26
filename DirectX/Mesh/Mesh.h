#pragma once

#include "../System/DirectXIncLib.h"
#include "../System/Game.h"
#include "../Utility/Collision.h"
#include "../Utility/Math.h"
#include <list>
#include <memory>
#include <string>
#include <vector>

class Buffer;
class MeshManager;
class Renderer;
class Texture;
class Transform3D;
class Shader;

enum class MeshState {
    ACTIVE,
    NON_ACTIVE,
    DEAD
};

struct MeshShaderConstantBuffer0 {
    Matrix4 world; //ワールド行列
    Matrix4 WVP; //ワールドから射影までの変換行列
    Vector4 lightDir; //ライト方向
    Vector4 eye; //カメラ位置
};

struct MeshShaderConstantBuffer1 {
    Vector4 ambient; //アンビエント光
    Vector4 diffuse; //ディフューズ色
    Vector4 specular; //鏡面反射
    Vector4 texture; //テクスチャーが貼られているメッシュかどうかのフラグ
};

//頂点の構造体
struct MeshVertex {
    Vector3 pos;
    Vector3 norm;
    Vector2 tex;
};

struct Material {
    std::string matName; //newmtl
    Vector4 Ka; //アンビエント
    Vector4 Kd; //ディフューズ
    Vector4 Ks; //スペキュラー
    std::string textureName; //テクスチャーファイル名
    ID3D11ShaderResourceView* texture;
    ID3D11SamplerState* sampleLinear;
    unsigned numFace; //そのマテリアルであるポリゴン数
    Material() {
        ZeroMemory(this, sizeof(Material));
    }
    ~Material() {
        SAFE_RELEASE(texture);
        SAFE_RELEASE(sampleLinear);
    }
};

class Mesh {
public:
    Mesh(std::shared_ptr<Renderer> renderer, const char* fileName);
    ~Mesh();
    void createSphere(std::shared_ptr<Sphere> sphere) const;
    void draw() const;
    void setTransform(std::shared_ptr<Transform3D> transform);
    MeshState getState() const;

    static void setMeshManager(MeshManager* manager);

private:
    bool loadMesh(const char* fileName);
    bool tempLoad(const char* fileName); //事前に頂点数などを調べる
    bool loadMaterial(const char* fileName, std::vector<Material>* material);
    void rendererMesh() const;
    std::string stringStrip(const std::string& string, const char delimiter);

private:
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<Transform3D> mTransform;
    std::shared_ptr<Shader> mShader;
    std::vector<Material> mMaterials;

    unsigned mNumVert; //頂点数
    unsigned mNumNormal; //法線数
    unsigned mNumTex; //テクスチャ座標数
    unsigned mNumFace; //ポリゴン数
    unsigned mNumMaterial; //マテリアル数

    std::vector<Vector3> mVertices; //頂点情報
    std::vector<Vector3> mNormals; //法線情報
    std::vector<Vector2> mTextures; //テクスチャ座標情報

    std::shared_ptr<Buffer> mVertexBuffer;
    std::vector<std::shared_ptr<Buffer>> mIndexBuffers;

    MeshState mState;

    static MeshManager* mMeshManager;
};
