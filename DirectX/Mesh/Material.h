#pragma once

#include "../Math/Math.h"
#include <memory>
#include <string>

class Texture;

struct Material {
    Vector3 ambient; //アンビエント
    Vector3 diffuse; //ディフューズ
    Vector3 specular; //スペキュラー
    Vector3 emissive;
    Vector3 bump;
    float transparency;
    float shininess;
    std::string matName; //newmtl
    std::string textureName; //テクスチャーファイル名
    std::shared_ptr<Texture> texture;
    unsigned numFace; //そのマテリアルであるポリゴン数

    Material() :
        ambient(Vector3::zero),
        diffuse(Vector3::zero),
        specular(Vector3::zero),
        emissive(Vector3::zero),
        bump(Vector3::zero),
        transparency(1.f),
        shininess(1.f),
        matName(""),
        textureName(""),
        texture(nullptr),
        numFace(0) {
    }
};
