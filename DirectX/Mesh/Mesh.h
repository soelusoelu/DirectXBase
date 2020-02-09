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
class Camera;
class MeshManager;
class Renderer;
class Texture;
class Transform3D;
class Shader;
class VertexArray;

enum class MeshState {
    ACTIVE,
    NON_ACTIVE,
    DEAD
};

struct MeshShaderConstantBuffer0 {
    ALIGN16 Matrix4 world; //ワールド行列
    ALIGN16 Matrix4 WVP; //ワールドから射影までの変換行列
    //ALIGN16 Vector3 lightPos; //ライト位置
    //ALIGN16 Matrix4 lightDir; //ライト方向
    //ALIGN16 Vector3 eye; //カメラ位置
};

//struct MeshShaderConstantBuffer1 {
//    ALIGN16 Vector4 ambient; //アンビエント光
//    ALIGN16 Vector4 diffuse; //ディフューズ色
//    ALIGN16 Vector4 specular; //鏡面反射
//    ALIGN16 float texture; //テクスチャーが貼られているメッシュかどうかのフラグ
//};

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
    std::shared_ptr<Texture> texture;
    unsigned numFace; //そのマテリアルであるポリゴン数
    Material() {
        ZeroMemory(this, sizeof(Material));
    }
};

class Mesh {
public:
    Mesh(std::shared_ptr<Renderer> renderer, const char* fileName);
    ~Mesh();
    void createSphere(std::shared_ptr<Sphere> sphere) const;
    static void drawAll(std::list<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera);
    void setTransform(std::shared_ptr<Transform3D> transform);
    void destroy();
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;

    static void setMeshManager(MeshManager* manager);

private:
    bool loadMesh(std::shared_ptr<Renderer> renderer, const char* fileName);
    bool tempLoad(std::shared_ptr<Renderer> renderer, const char* fileName); //事前に頂点数などを調べる
    bool loadMaterial(std::shared_ptr<Renderer> renderer, const char* fileName, std::vector<std::unique_ptr<Material>>* materials);
    void renderMesh(std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera) const;
    static void renderToTexture(std::shared_ptr<Renderer> renderer);
    static void renderFromTexture(std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera);
    std::string stringStrip(const std::string& string, const char delimiter);

private:
    std::shared_ptr<Transform3D> mTransform;
    std::shared_ptr<Shader> mShader;
    std::vector<std::unique_ptr<Material>> mMaterials;
    std::unique_ptr<VertexArray> mVertexArray;

    MeshState mState;

    static MeshManager* mMeshManager;
};
