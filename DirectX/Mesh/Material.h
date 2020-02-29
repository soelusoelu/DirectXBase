#pragma once

#include "../Utility/Math.h"
#include <memory>
#include <string>

class Texture;

struct Material {
    Vector4 ambient; //アンビエント
    Vector4 diffuse; //ディフューズ
    Vector4 specular; //スペキュラー
    Vector4 emissive;
    Vector4 bump;
    float transparency;
    float shininess;
    std::string matName; //newmtl
    std::string textureName; //テクスチャーファイル名
    std::shared_ptr<Texture> texture;
    unsigned numFace; //そのマテリアルであるポリゴン数

    Material() :
        ambient(Vector3::zero, 1.f),
        diffuse(Vector3::zero, 1.f),
        specular(Vector3::zero, 1.f),
        emissive(Vector3::zero, 1.f),
        bump(Vector3::zero, 1.f),
        transparency(1.f),
        shininess(1.f),
        matName(""),
        textureName(""),
        texture(nullptr),
        numFace(0) {
    }
};
