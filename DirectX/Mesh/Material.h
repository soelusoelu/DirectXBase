#pragma once

#include "../System/Game.h"
#include "../Utility/Math.h"
#include <fstream>
#include <memory>
#include <string>
#include <vector>

class Renderer;
class Texture;

struct MaterialData {
    std::string matName; //newmtl
    Vector4 Ka; //アンビエント
    Vector4 Kd; //ディフューズ
    Vector4 Ks; //スペキュラー
    std::string textureName; //テクスチャーファイル名
    std::shared_ptr<Texture> texture;
    unsigned numFace; //そのマテリアルであるポリゴン数
};

class Material {
    using MaterialPtr = std::shared_ptr<MaterialData>;
    using MaterialPtrArray = std::vector<MaterialPtr>;

public:
    Material();
    ~Material();
    bool load(std::shared_ptr<Renderer> renderer, const std::string& fileName);
    unsigned getNumMaterials() const;
    MaterialPtr getMaterialData(unsigned index) const;

private:
    bool preload(std::ifstream& stream, const std::string& fileName);

private:
    MaterialPtrArray mMaterials;
    unsigned mNumMaterials;
};
