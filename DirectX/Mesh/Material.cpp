#include "Material.h"
#include "../Device/AssetsManager.h"
#include "../Utility/StringUtil.h"

Material::Material() = default;

Material::~Material() = default;

unsigned Material::getNumMaterials() const {
    return mMaterials.size();
}

std::shared_ptr<MaterialData> Material::getMaterialData(unsigned index) const {
    return mMaterials[index];
}

bool Material::load(std::shared_ptr<AssetsManager> assetsManager, const std::string & fileName) {
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
    char s[256]; //ダミー
    Vector4 v(0.f, 0.f, 0.f, 1.f);
    int matCount = -1;

    //本読み込み
    while (!ifs.eof()) {
        //キーワード読み込み
        std::getline(ifs, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        sscanf_s(line.c_str(), "%s", s, sizeof(s));

        //マテリアル名
        if (strcmp(s, "newmtl") == 0) {
            matCount++;
            mMaterials[matCount]->matName = line.substr(7); //「newmtl 」の文字数分
        }
        //Ka アンビエント
        if (strcmp(s, "Ka") == 0) {
            sscanf_s(line.c_str(), "%s %f %f %f", s, sizeof(s), &v.x, &v.y, &v.z);
            mMaterials[matCount]->Ka = v;
        }
        //Kd ディフューズ
        if (strcmp(s, "Kd") == 0) {
            sscanf_s(line.c_str(), "%s %f %f %f", s, sizeof(s), &v.x, &v.y, &v.z);
            mMaterials[matCount]->Kd = v;
        }
        //Ks スペキュラー
        if (strcmp(s, "Ks") == 0) {
            sscanf_s(line.c_str(), "%s %f %f %f", s, sizeof(s), &v.x, &v.y, &v.z);
            mMaterials[matCount]->Ks = v;
        }
        //map_Kd テクスチャー
        if (strcmp(s, "map_Kd") == 0) {
            mMaterials[matCount]->textureName = line.substr(7); //「map_Kd 」の文字数分
            //テクスチャーを作成
            mMaterials[matCount]->texture = assetsManager->createTexture(mMaterials[matCount]->textureName, false);
        }
    }

    return true;
}

bool Material::preload(std::ifstream & stream, const std::string & fileName) {
    //マテリアルファイルを開いて内容を読み込む
    std::string line;
    char s[256];
    while (!stream.eof()) {
        //キーワード読み込み
        std::getline(stream, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        sscanf_s(line.c_str(), "%s", s, sizeof(s));

        //マテリアル名
        if (strcmp(s, "newmtl") == 0) {
            mMaterials.emplace_back(std::make_shared<MaterialData>());
        }
    }

    if (mMaterials.empty()) {
        MSG(L"マテリアルが空です");
        return false;
    }

    return true;
}
