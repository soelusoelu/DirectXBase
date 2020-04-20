#include "OBJ.h"
#include "Material.h"
#include "VertexArray.h"
#include "../DebugLayer/Debug.h"
#include "../Device/AssetsManager.h"
#include "../System/Game.h"
#include "../Utility/Directory.h"
#include "../Utility/FileUtil.h"
#include "../Utility/StringUtil.h"

OBJ::OBJ() :
    mVertexArray(std::make_shared<VertexArray>()),
    mVertices(nullptr) {
}

OBJ::~OBJ() {
    SAFE_DELETE_ARRAY(mVertices);
}

void OBJ::perse(const std::string& filePath) {
    //OBJファイルを開いて内容を読み込む
    Singleton<Directory>::instance().setModelDirectory(filePath);
    auto fileName = FileUtil::getFileNameFromDirectry(filePath);
    std::ifstream ifs(fileName, std::ios::in);
    if (ifs.fail()) {
        Debug::windowMessage(filePath + "ファイルが存在しません");
        return;
    }

    //事前に頂点数などを調べる
    if (!preload(ifs, fileName)) {
        Debug::windowMessage(filePath + "ファイルの事前読み込み失敗");
        return;
    }

    //サイズ変更
    Vector3* vertices = new Vector3[mVertexArray->getNumVerts()];
    Vector3* normals = new Vector3[mVertexArray->getNumNormal()];
    Vector2* textures = new Vector2[mVertexArray->getNumTex()];

    //本読み込み
    ifs.clear();
    ifs.seekg(0, std::ios_base::beg);
    std::string line;
    unsigned vCount = 0;
    unsigned vnCount = 0;
    unsigned vtCount = 0;
    char s[256]; //ダミー
    float x, y, z;

    while (!ifs.eof()) {
        //キーワード読み込み
        std::getline(ifs, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        //空白が来るまで読み込み
        sscanf_s(line.c_str(), "%s", s, sizeof(s));

        //頂点読み込み
        if (strcmp(s, "v") == 0) {
            sscanf_s(line.c_str(), "%s %f %f %f", s, sizeof(s), &x, &y, &z);
            vertices[vCount].x = -x;
            vertices[vCount].y = y;
            vertices[vCount].z = z;
            vCount++;
        }

        //法線読み込み
        if (strcmp(s, "vn") == 0) {
            sscanf_s(line.c_str(), "%s %f %f %f", s, sizeof(s), &x, &y, &z);
            normals[vnCount].x = -x;
            normals[vnCount].y = y;
            normals[vnCount].z = z;
            vnCount++;
        }

        //テクスチャー座標 読み込み
        if (strcmp(s, "vt") == 0) {
            sscanf_s(line.c_str(), "%s %f %f", s, sizeof(s), &x, &y);
            textures[vtCount].x = x;
            textures[vtCount].y = 1.f - y; //OBJファイルはY成分が逆なので合わせる
            vtCount++;
        }
    }

    //フェイス読み込み バラバラに収録されている可能性があるので、マテリアル名を頼りにつなぎ合わせる
    bool matFlag = false;
    int* faceBuffer = new int[mVertexArray->getNumFace() * 3]; //3頂点ポリゴンなので、1フェイス=3頂点(3インデックス)

    mVertices = new MeshVertex[mVertexArray->getNumVerts()];
    int v1 = 0, v2 = 0, v3 = 0;
    int vn1 = 0, vn2 = 0, vn3 = 0;
    int vt1 = 0, vt2 = 0, vt3 = 0;
    int fCount = 0;

    for (unsigned i = 0; i < mInitMaterials.size(); i++) {
        ifs.clear();
        ifs.seekg(0, std::ios_base::beg);
        fCount = 0;

        while (!ifs.eof()) {
            //キーワード 読み込み
            std::getline(ifs, line);

            if (line.empty() || line[0] == '#') {
                continue;
            }

            //空白が来るまで読み込み
            sscanf_s(line.c_str(), "%s", s, sizeof(s));

            //フェイス 読み込み→頂点インデックスに
            if (strcmp(s, "usemtl") == 0) {
                auto mat = line.substr(7); //「usemtl 」の文字数分
                matFlag = (mInitMaterials[i]->matName == mat);
            }

            if (strcmp(s, "f") == 0 && matFlag) {
                if (mInitMaterials[i]->texture) { //テクスチャーありサーフェイス
                    sscanf_s(line.c_str(), "%s %d/%d/%d %d/%d/%d %d/%d/%d", s, sizeof(s), &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
                } else { //テクスチャー無しサーフェイス
                    sscanf_s(line.c_str(), "%s %d//%d %d//%d %d//%d", s, sizeof(s), &v1, &vn1, &v2, &vn2, &v3, &vn3);
                }

                //テクスチャー座標 > 頂点数がありえる
                int index1 = (vtCount > vCount) ? vt1 : v1;
                int index2 = (vtCount > vCount) ? vt2 : v2;
                int index3 = (vtCount > vCount) ? vt3 : v3;

                //フェイス 読み込み→頂点インデックスに
                faceBuffer[fCount * 3] = index1 - 1;
                faceBuffer[fCount * 3 + 1] = index2 - 1;
                faceBuffer[fCount * 3 + 2] = index3 - 1;
                fCount++;
                //頂点構造体に代入
                mVertices[index1 - 1].pos = vertices[v1 - 1];
                mVertices[index1 - 1].normal = normals[vn1 - 1];
                mVertices[index1 - 1].uv = textures[vt1 - 1];
                mVertices[index2 - 1].pos = vertices[v2 - 1];
                mVertices[index2 - 1].normal = normals[vn2 - 1];
                mVertices[index2 - 1].uv = textures[vt2 - 1];
                mVertices[index3 - 1].pos = vertices[v3 - 1];
                mVertices[index3 - 1].normal = normals[vn3 - 1];
                mVertices[index3 - 1].uv = textures[vt3 - 1];
            }
        }
        if (fCount == 0) { //使用されていないマテリアル対策
            continue;
        }

        //インデックスバッファーを作成
        mVertexArray->createIndexBuffer(i, fCount * 3, faceBuffer);

        mInitMaterials[i]->numFace = fCount;
    }

    delete[] vertices;
    delete[] normals;
    delete[] textures;
    delete[] faceBuffer;

    //バーテックスバッファーを作成
    mVertexArray->createVertexBuffer(sizeof(MeshVertex), mVertices);
}

void OBJ::setInitMaterials(MaterialPtrArray* rhs) const {
    if (!rhs->empty()) {
        rhs->clear();
    }

    auto mat = std::make_shared<Material>();
    for (const auto& lhs : mInitMaterials) {
        mat->ambient = lhs->ambient;
        mat->diffuse = lhs->diffuse;
        mat->specular = lhs->specular;
        mat->emissive = lhs->emissive;
        mat->bump = lhs->bump;
        mat->transparency = lhs->transparency;
        mat->shininess = lhs->shininess;
        mat->matName = lhs->matName;
        mat->textureName = lhs->textureName;
        mat->texture = lhs->texture;
        mat->numFace = lhs->numFace;

        rhs->emplace_back(mat);
    }
}

std::shared_ptr<VertexArray> OBJ::getVertexArray() const {
    return mVertexArray;
}

Vector3 OBJ::getCenter() const {
    auto min = Vector3::one * Math::infinity;
    auto max = Vector3::one * Math::negInfinity;
    for (size_t i = 0; i < mVertexArray->getNumVerts(); i++) {
        if (mVertices[i].pos.x < min.x) {
            min.x = mVertices[i].pos.x;
        }
        if (mVertices[i].pos.x > max.x) {
            max.x = mVertices[i].pos.x;
        }
        if (mVertices[i].pos.y < min.y) {
            min.y = mVertices[i].pos.y;
        }
        if (mVertices[i].pos.y > max.y) {
            max.y = mVertices[i].pos.y;
        }
        if (mVertices[i].pos.z < min.z) {
            min.z = mVertices[i].pos.z;
        }
        if (mVertices[i].pos.z > max.z) {
            max.z = mVertices[i].pos.z;
        }
    }
    return (max + min) / 2.f;
}

float OBJ::getRadius() const {
    float min = Math::infinity;
    float max = Math::negInfinity;
    for (size_t i = 0; i < mVertexArray->getNumVerts(); i++) {
        if (mVertices[i].pos.x < min) {
            min = mVertices[i].pos.x;
        }
        if (mVertices[i].pos.x > max) {
            max = mVertices[i].pos.x;
        }
        if (mVertices[i].pos.y < min) {
            min = mVertices[i].pos.y;
        }
        if (mVertices[i].pos.y > max) {
            max = mVertices[i].pos.y;
        }
        if (mVertices[i].pos.z < min) {
            min = mVertices[i].pos.z;
        }
        if (mVertices[i].pos.z > max) {
            max = mVertices[i].pos.z;
        }
    }
    return (max - min) / 2.f;
}

bool OBJ::preload(std::ifstream& stream, const std::string& filePath) {
    //OBJファイルを開いて内容を読み込む
    unsigned numVert = 0;
    unsigned numNormal = 0;
    unsigned numTex = 0;
    unsigned numFace = 0;
    std::string line;
    char s[256];
    //事前に頂点数、ポリゴン数を調べる
    while (!stream.eof()) {
        //キーワード読み込み
        std::getline(stream, line);

        if (line.empty() || line[0] == '#') { //空かコメントならパス
            continue;
        }

        //空白が来るまで読み込み
        sscanf_s(line.c_str(), "%s", s, sizeof(s));

        //マテリアル読み込み
        if (strcmp(s, "mtllib") == 0) {
            auto mat = line.substr(7); //「mtllib 」の文字数分
            if (!materialLoad(mat, filePath)) {
                return false;
            }
        }
        //頂点
        if (strcmp(s, "v") == 0) {
            numVert++;
        }
        //法線
        if (strcmp(s, "vn") == 0) {
            numNormal++;
        }
        //テクスチャー座標
        if (strcmp(s, "vt") == 0) {
            numTex++;
        }
        //フェイス(ポリゴン)
        if (strcmp(s, "f") == 0) {
            numFace++;
        }
    }
    //テクスチャー座標 > 頂点数がありえる。その場合、頂点を増やす必要がある
    if (numTex > numVert) {
        numVert = numTex;
    }

    mVertexArray->setNumVerts(numVert);
    mVertexArray->setNumNormal(numNormal);
    mVertexArray->setNumTex(numTex);
    mVertexArray->setNumFace(numFace);

    return true;
}

bool OBJ::materialLoad(const std::string& fileName, const std::string& filePath) {
    std::ifstream ifs(fileName, std::ios::in);
    if (ifs.fail()) {
        Debug::windowMessage(FileUtil::getDirectryFromFilePath(filePath) + "/" + fileName + "ファイルが存在しません");
        return false;
    }

    //マテリアルの事前読み込み
    materialPreload(ifs);

    //ファイルの先頭に戻る
    ifs.clear();
    ifs.seekg(0, std::ios_base::beg);

    std::string line;
    char s[256]; //ダミー
    Vector3 v;
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
            mInitMaterials[matCount]->matName = line.substr(7); //「newmtl 」の文字数分
        }
        //アンビエント
        if (strcmp(s, "Ka") == 0) {
            sscanf_s(line.c_str(), "%s %f %f %f", s, sizeof(s), &v.x, &v.y, &v.z);
            mInitMaterials[matCount]->ambient = v;
        }
        //ディフューズ
        if (strcmp(s, "Kd") == 0) {
            sscanf_s(line.c_str(), "%s %f %f %f", s, sizeof(s), &v.x, &v.y, &v.z);
            mInitMaterials[matCount]->diffuse = v;
        }
        //スペキュラー
        if (strcmp(s, "Ks") == 0) {
            sscanf_s(line.c_str(), "%s %f %f %f", s, sizeof(s), &v.x, &v.y, &v.z);
            mInitMaterials[matCount]->specular = v;
        }
        //テクスチャー
        if (strcmp(s, "map_Kd") == 0) {
            mInitMaterials[matCount]->textureName = line.substr(7); //「map_Kd 」の文字数分

            auto dir = FileUtil::getDirectryFromFilePath(filePath);
            dir += "/" + mInitMaterials[matCount]->textureName;

            //テクスチャーを作成
            mInitMaterials[matCount]->texture = Singleton<AssetsManager>::instance().createTexture(dir, false);
        }
    }

    return true;
}

void OBJ::materialPreload(std::ifstream& stream) {
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
            mInitMaterials.emplace_back(std::make_shared<Material>());
        }
    }

    return;
}
