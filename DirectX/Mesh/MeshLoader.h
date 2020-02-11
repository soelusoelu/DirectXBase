#pragma once

#include "../System/DirectXIncLib.h"
#include "../Utility/Collision.h"
#include "../Utility/Math.h"
#include <fstream>
#include <memory>
#include <string>
#include <vector>

class Renderer;
class Texture;
class VertexArray;

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

class MeshLoader {
    using MaterialPtr = std::shared_ptr<Material>;
    using MaterialPtrArray = std::vector<MaterialPtr>;

public:
    MeshLoader(std::shared_ptr<Renderer> renderer, const std::string& fileName);
    ~MeshLoader();
    void createSphere(std::shared_ptr<Sphere>* sphere) const;
    void setVertexBuffer(unsigned vertexSize, unsigned numStream = 1, unsigned start = 0);
    void setIndexBuffer(unsigned index, unsigned offset = 0);
    size_t getMaterialSize() const;
    MaterialPtr getMaterial(unsigned index) const;

private:
    bool loadMesh(std::shared_ptr<Renderer> renderer, const std::string& fileName);
    bool tempLoad(std::ifstream& stream, std::shared_ptr<Renderer> renderer, const std::string& fileName); //事前に頂点数などを調べる
    bool loadMaterial(std::shared_ptr<Renderer> renderer, const std::string& fileName, MaterialPtrArray* materials);
    bool tempLoadMaterial(std::ifstream& stream, const std::string& fileName);
    std::string stringStrip(const std::string& string, const char delimiter);

private:
    std::unique_ptr<VertexArray> mVertexArray;
    MaterialPtrArray mMaterials;
    unsigned mNumMaterials;
};
