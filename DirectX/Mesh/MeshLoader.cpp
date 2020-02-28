#include "MeshLoader.h"
#include "Material.h"
#include "../System/Game.h"
#include "../System/VertexArray.h"
#include "../Utility/StringUtil.h"

MeshLoader::MeshLoader(std::shared_ptr<AssetsManager> assetsManager, const std::string& fileName) :
    mMaterial(std::make_unique<Material>()),
    mVertexArray(std::make_unique<VertexArray>()) {
    if (!load(assetsManager, fileName)) {
        MSG(L"メッシュ作成失敗");
        return;
    }
}

MeshLoader::~MeshLoader() = default;

void MeshLoader::createSphere(std::shared_ptr<Sphere> * sphere) const {
    //バウンディングスフィア作成
    auto verts = mVertexArray->getVertices();
    float min = Math::infinity;
    float max = Math::negInfinity;
    Vector3 minVec3 = Vector3::one * Math::infinity;
    Vector3 maxVec3 = Vector3::one * Math::negInfinity;
    for (size_t i = 0; i < mVertexArray->getNumVerts(); i++) {
        //半径
        if (verts[i].x < min) {
            min = verts[i].x;
        }
        if (verts[i].x > max) {
            max = verts[i].x;
        }
        if (verts[i].y < min) {
            min = verts[i].y;
        }
        if (verts[i].y > max) {
            max = verts[i].y;
        }
        if (verts[i].z < min) {
            min = verts[i].z;
        }
        if (verts[i].z > max) {
            max = verts[i].z;
        }

        //中心
        if (verts[i].x < minVec3.x) {
            minVec3.x = verts[i].x;
        }
        if (verts[i].x > maxVec3.x) {
            maxVec3.x = verts[i].x;
        }
        if (verts[i].y < minVec3.y) {
            minVec3.y = verts[i].y;
        }
        if (verts[i].y > maxVec3.y) {
            maxVec3.y = verts[i].y;
        }
        if (verts[i].z < minVec3.z) {
            minVec3.z = verts[i].z;
        }
        if (verts[i].z > maxVec3.z) {
            maxVec3.z = verts[i].z;
        }
    }
    float r = (max - min) / 2.f;
    (*sphere)->radius = r;
    auto c = (maxVec3 + minVec3) / 2.f;
    (*sphere)->center = c;
}

void MeshLoader::setVertexBuffer(unsigned numStream, unsigned start, unsigned offset) {
    mVertexArray->setVertexBuffer(numStream, start, offset);
}

void MeshLoader::setIndexBuffer(unsigned index, unsigned offset) {
    mVertexArray->setIndexBuffer(index, offset);
}

unsigned MeshLoader::getMaterialSize() const {
    return mMaterial->getNumMaterials();
}

std::shared_ptr<MaterialData> MeshLoader::getMaterialData(unsigned index) const {
    return mMaterial->getMaterialData(index);
}

bool MeshLoader::load(std::shared_ptr<AssetsManager> assetsManager, const std::string & fileName) {
    //OBJファイルを開いて内容を読み込む
    setOBJDirectory();
    std::ifstream ifs(fileName, std::ios::in);
    if (ifs.fail()) {
        MSG(L"OBJファイルが存在しません");
        return false;
    }

    //事前に頂点数などを調べる
    if (!preload(ifs, assetsManager, fileName)) {
        MSG(L"Meshファイルの事前読み込み失敗");
        return false;
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
            textures[vtCount].y = 1 - y; //OBJファイルはY成分が逆なので合わせる
            vtCount++;
        }
    }

    //マテリアルの数だけインデックスバッファーを作成
    mVertexArray->resizeIndexBuffer(mMaterial->getNumMaterials());
    //頂点情報を記録
    mVertexArray->setVertices(vertices);

    //フェイス読み込み バラバラに収録されている可能性があるので、マテリアル名を頼りにつなぎ合わせる
    bool matFlag = false;
    int* faceBuffer = new int[mVertexArray->getNumFace() * 3]; //3頂点ポリゴンなので、1フェイス=3頂点(3インデックス)

    auto* meshVertices = new MeshVertex[mVertexArray->getNumVerts()];
    int v1 = 0, v2 = 0, v3 = 0;
    int vn1 = 0, vn2 = 0, vn3 = 0;
    int vt1 = 0, vt2 = 0, vt3 = 0;
    int fCount = 0;

    for (unsigned i = 0; i < mMaterial->getNumMaterials(); i++) {
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
                matFlag = (mMaterial->getMaterialData(i)->matName == mat);
            }

            if (strcmp(s, "f") == 0 && matFlag) {
                if (mMaterial->getMaterialData(i)->texture) { //テクスチャーありサーフェイス
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
                meshVertices[index1 - 1].pos = vertices[v1 - 1];
                meshVertices[index1 - 1].norm = normals[vn1 - 1];
                meshVertices[index1 - 1].tex = textures[vt1 - 1];
                meshVertices[index2 - 1].pos = vertices[v2 - 1];
                meshVertices[index2 - 1].norm = normals[vn2 - 1];
                meshVertices[index2 - 1].tex = textures[vt2 - 1];
                meshVertices[index3 - 1].pos = vertices[v3 - 1];
                meshVertices[index3 - 1].norm = normals[vn3 - 1];
                meshVertices[index3 - 1].tex = textures[vt3 - 1];
            }
        }
        if (fCount == 0) { //使用されていないマテリアル対策
            continue;
        }

        //インデックスバッファーを作成
        mVertexArray->createIndexBuffer(i, fCount * 3, faceBuffer);

        mMaterial->getMaterialData(i)->numFace = fCount;
    }

    delete[] normals;
    delete[] textures;
    delete[] faceBuffer;

    //バーテックスバッファーを作成
    mVertexArray->createVertexBuffer(sizeof(MeshVertex), meshVertices);

    delete[] meshVertices;

    return true;
}

bool MeshLoader::preload(std::ifstream & stream, std::shared_ptr<AssetsManager> assetsManager, const std::string & fileName) {
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
            if (!mMaterial->load(assetsManager, mat)) {
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
