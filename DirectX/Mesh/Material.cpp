#include "Material.h"
#include "../Device/Renderer.h"
#include "../Utility/StringUtil.h"

Material::Material() = default;

Material::~Material() = default;

unsigned Material::getNumMaterials() const {
    return mMaterials.size();
}

std::shared_ptr<MaterialData> Material::getMaterialData(unsigned index) const {
    return mMaterials[index];
}

bool Material::load(std::shared_ptr<Renderer> renderer, const std::string & fileName) {
    std::ifstream ifs(fileName, std::ios::in);
    if (ifs.fail()) {
        MSG(L"mtlファイルが存在しません");
        return false;
    }

    //マテリアルの事前読み込み
    if (!preload(ifs, fileName)) {
        MSG(L"mtlファイルの事前読み込み失敗");
        return false;
    }

    //ファイルの先頭に戻る
    ifs.clear();
    ifs.seekg(0, std::ios_base::beg);

    std::string line;
    std::string strip;
    Vector4 v(0.f, 0.f, 0.f, 1.f);
    int matCount = -1;

    //本読み込み
    while (!ifs.eof()) {
        //キーワード読み込み
        std::getline(ifs, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        strip = StringUtil::spritFirst(line, ' ');

        //マテリアル名
        if (strip == "newmtl") {
            matCount++;
            mMaterials[matCount]->matName = line.substr(7); //「newmtl 」の文字数分
        }
        //Ka アンビエント
        if (strip == "Ka") {
            auto sub = line.substr(3); //「Ka 」の文字数分
            sscanf_s(sub.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
            mMaterials[matCount]->Ka = v;
        }
        //Kd ディフューズ
        if (strip == "Kd") {
            auto sub = line.substr(3); //「Kd 」の文字数分
            auto i = sscanf_s(sub.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
            mMaterials[matCount]->Kd = v;
        }
        //Ks スペキュラー
        if (strip == "Ks") {
            auto sub = line.substr(3); //「Ks 」の文字数分
            sscanf_s(sub.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
            mMaterials[matCount]->Ks = v;
        }
        //map_Kd テクスチャー
        if (strip == "map_Kd") {
            mMaterials[matCount]->textureName = line.substr(7); //「map_Kd 」の文字数分
            //テクスチャーを作成
            mMaterials[matCount]->texture = renderer->createTexture(mMaterials[matCount]->textureName, false);
        }
    }

    return true;
}

bool Material::preload(std::ifstream & stream, const std::string & fileName) {
    //マテリアルファイルを開いて内容を読み込む
    std::string line;
    std::string strip;
    while (!stream.eof()) {
        //キーワード読み込み
        std::getline(stream, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        strip = StringUtil::spritFirst(line, ' ');

        //マテリアル名
        if (strip == "newmtl") {
            mMaterials.emplace_back(std::make_shared<MaterialData>());
        }
    }

    if (mMaterials.empty()) {
        MSG(L"マテリアルが空です");
        return false;
    }

    return true;
}
