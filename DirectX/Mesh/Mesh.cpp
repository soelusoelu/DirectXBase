#include "Mesh.h"
#include "MeshManager.h"
#include "../Actor/Transform3D.h"
#include "../Device/Renderer.h"
#include "../Shader/Shader.h"
#include "../System/Buffer.h"
#include "../System/BufferDesc.h"
#include "../System/Direct3D11.h"
#include "../System/DirectXIncLib.h"
#include "../System/Game.h"
#include "../System/InputElement.h"
#include "../System/SubResourceDesc.h"
#include "../System/VertexStreamDesc.h"
#include <fstream>
#include <string>
#include <stdio.h>

Mesh::Mesh(std::shared_ptr<Renderer> renderer, const char* fileName) :
    mRenderer(renderer),
    mTransform(nullptr),
    mShader(renderer->createShader("Mesh.hlsl")),
    mMaterials(0),
    mNumVert(0),
    mNumNormal(0),
    mNumTex(0),
    mNumFace(0),
    mNumMaterial(0),
    mVertices(0),
    mNormals(0),
    mTextures(0),
    mVertexBuffer(nullptr),
    mIndexBuffers(0),
    mState(MeshState::ACTIVE) {
    if (!loadMesh(fileName)) {
        MSG(L"メッシュ作成失敗");
        return;
    }

    //メッシュ用コンスタントバッファの作成
    mShader->createConstantBuffer(sizeof(MeshShaderConstantBuffer0), 0);
    mShader->createConstantBuffer(sizeof(MeshShaderConstantBuffer1), 1);

    //インプットレイアウトの生成
    constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 6, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    mShader->createInputLayout(layout, numElements);

    if (mMeshManager) {
        mMeshManager->add(this);
    }
}

Mesh::~Mesh() = default;

void Mesh::createSphere(std::shared_ptr<Sphere> sphere) const {
    //バウンディングスフィア作成
    //D3DXVECTOR3 center;
    //D3DXVECTOR3 first(mCoord->x, mCoord->y, mCoord->z);
    //D3DXComputeBoundingSphere(&first, mNumVert, sizeof(D3DXVECTOR3), &center, &sphere->radius);
    //sphere->center.x = center.x;
    //sphere->center.y = center.y;
    //sphere->center.z = center.z;
}

void Mesh::draw() const {
    //プリミティブ・トポロジーをセット
    mRenderer->setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_STRIP);

    mRenderer->deviceContext()->RSSetState(mRenderer->rasterizerState());
    rendererMesh();
    mRenderer->deviceContext()->RSSetState(mRenderer->rasterizerStateBack());
    rendererMesh();
}

void Mesh::setTransform(std::shared_ptr<Transform3D> transform) {
    mTransform = transform;
}

MeshState Mesh::getState() const {
    return mState;
}

void Mesh::setMeshManager(MeshManager * manager) {
    mMeshManager = manager;
}

bool Mesh::loadMesh(const char* fileName) {
    //事前に頂点数などを調べる
    if (!tempLoad(fileName)) {
        MSG(L"Meshファイルの事前読み込み失敗");
        return false;
    }

    //サイズ変更
    auto* meshVertices = new MeshVertex[mNumVert];
    mVertices.resize(mNumVert);
    mNormals.resize(mNumNormal);
    mTextures.resize(mNumTex);

    //OBJファイルを開いて内容を読み込む
    setOBJDirectory();
    std::ifstream ifs(fileName, std::ios::in);

    std::string line;
    std::string strip;
    unsigned vCount = 0;
    unsigned vnCount = 0;
    unsigned vtCount = 0;
    float x, y, z;

    //本読み込み
    while (!ifs.eof()) {
        //キーワード読み込み
        std::getline(ifs, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        //空白が来るまで読み込み
        strip = stringStrip(line, ' ');

        //頂点読み込み
        if (strip == "v") {
            auto sub = line.substr(2); //「v 」の文字数分
            sscanf_s(sub.c_str(), "%f %f %f", &x, &y, &z);
            mVertices[vCount].x = -x;
            mVertices[vCount].y = y;
            mVertices[vCount].z = z;
            vCount++;
        }

        //法線読み込み
        if (strip == "vn") {
            auto sub = line.substr(3); //「vn 」の文字数分
            sscanf_s(sub.c_str(), "%f %f %f", &x, &y, &z);
            mNormals[vnCount].x = -x;
            mNormals[vnCount].y = y;
            mNormals[vnCount].z = z;
            vnCount++;
        }

        //テクスチャー座標 読み込み
        if (strip == "vt") {
            auto sub = line.substr(3); //「vt 」の文字数分
            sscanf_s(sub.c_str(), "%f %f", &x, &y);
            mTextures[vtCount].x = x;
            mTextures[vtCount].y = -y; //OBJファイルはY成分が逆なので合わせる
            vtCount++;
        }
    }

    //マテリアルの数だけインデックスバッファーを作成
    mIndexBuffers.resize(mNumMaterial);

    //フェイス読み込み バラバラに収録されている可能性があるので、マテリアル名を頼りにつなぎ合わせる
    bool matFlag = false;
    int* faceBuffer = new int[mNumFace * 3]; //3頂点ポリゴンなので、1フェイス=3頂点(3インデックス)

    int v1 = 0, v2 = 0, v3 = 0;
    int vn1 = 0, vn2 = 0, vn3 = 0;
    int vt1 = 0, vt2 = 0, vt3 = 0;
    unsigned fCount = 0;

    for (unsigned i = 0; i < mNumMaterial; i++) {
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
            strip = stringStrip(line, ' ');

            //フェイス 読み込み→頂点インデックスに
            if (strip == "usemtl") {
                auto mat = line.substr(7); //「usemtl 」の文字数分
                matFlag = (mMaterials[i].matName == mat);
            }

            if (strip == "f" && matFlag) {
                auto sub = line.substr(2); //「f 」の文字数分
                if (mMaterials[i].texture) { //テクスチャーありサーフェイス
                    sscanf_s(sub.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
                } else { //テクスチャー無しサーフェイス
                    sscanf_s(sub.c_str(), "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
                }

                //テクスチャー座標 > 頂点数がありえる
                if (vtCount > vCount) {
                    //フェイス 読み込み→頂点インデックスに テクスチャー座標インデックスを基準にする(テクスチャー座標 >= 頂点座標なので)
                    sscanf_s(sub.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
                    faceBuffer[fCount * 3] = vt1 - 1;
                    faceBuffer[fCount * 3 + 1] = vt2 - 1;
                    faceBuffer[fCount * 3 + 2] = vt3 - 1;
                    fCount++;
                    //頂点構造体に代入 テクスチャー座標インデックスを基準にする(テクスチャー座標 >= 頂点座標なので)
                    meshVertices[vt1 - 1].pos = mVertices[v1 - 1];
                    meshVertices[vt1 - 1].norm = mNormals[vn1 - 1];
                    meshVertices[vt1 - 1].tex = mTextures[vt1 - 1];
                    meshVertices[vt2 - 1].pos = mVertices[v2 - 1];
                    meshVertices[vt2 - 1].norm = mNormals[vn2 - 1];
                    meshVertices[vt2 - 1].tex = mTextures[vt2 - 1];
                    meshVertices[vt3 - 1].pos = mVertices[v3 - 1];
                    meshVertices[vt3 - 1].norm = mNormals[vn3 - 1];
                    meshVertices[vt3 - 1].tex = mTextures[vt3 - 1];
                } else {
                    //フェイス 読み込み→頂点インデックスに
                    sscanf_s(sub.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
                    faceBuffer[fCount * 3] = v1 - 1;
                    faceBuffer[fCount * 3 + 1] = v2 - 1;
                    faceBuffer[fCount * 3 + 2] = v3 - 1;
                    fCount++;
                    //頂点構造体に代入
                    meshVertices[v1 - 1].pos = mVertices[v1 - 1];
                    meshVertices[v1 - 1].norm = mNormals[vn1 - 1];
                    meshVertices[v1 - 1].tex = mTextures[vt1 - 1];
                    meshVertices[v2 - 1].pos = mVertices[v2 - 1];
                    meshVertices[v2 - 1].norm = mNormals[vn2 - 1];
                    meshVertices[v2 - 1].tex = mTextures[vt2 - 1];
                    meshVertices[v3 - 1].pos = mVertices[v3 - 1];
                    meshVertices[v3 - 1].norm = mNormals[vn3 - 1];
                    meshVertices[v3 - 1].tex = mTextures[vt3 - 1];
                }
            }
        }
        if (fCount == 0) { //使用されていないマテリアル対策
            mIndexBuffers[i] = nullptr;
            continue;
        }

        //インデックスバッファーを作成
        BufferDesc bd;
        bd.size = sizeof(int) * fCount * 3;
        bd.usage = BufferUsage::BUFFER_USAGE_DEFAULT;
        bd.type = BufferType::BUFFER_TYPE_INDEX;
        SubResourceDesc sub;
        sub.data = faceBuffer;

        mIndexBuffers[i] = mRenderer->createBuffer(bd, &sub);

        mMaterials[i].numFace = fCount;
    }

    delete[] faceBuffer;

    //バーテックスバッファーを作成
    BufferDesc bd;
    bd.size = sizeof(MeshVertex) * vCount;
    bd.usage = BufferUsage::BUFFER_USAGE_DEFAULT;
    bd.type = BufferType::BUFFER_TYPE_VERTEX;
    SubResourceDesc sub;
    sub.data = meshVertices;

    mVertexBuffer = mRenderer->createBuffer(bd, &sub);

    delete[] meshVertices;

    return true;
}

bool Mesh::tempLoad(const char* fileName) {
    //OBJファイルを開いて内容を読み込む
    setOBJDirectory();
    std::ifstream ifs(fileName, std::ios::in);
    if (ifs.fail()) {
        MSG(L"OBJファイルが存在しません");
        return false;
    }

    std::string line;
    std::string strip;
    //事前に頂点数、ポリゴン数を調べる
    while (!ifs.eof()) {
        //キーワード読み込み
        std::getline(ifs, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        //空白が来るまで読み込み
        strip = stringStrip(line, ' ');

        //マテリアル読み込み
        if (strip == "mtllib") {
            auto mat = line.substr(7); //「mtllib 」の文字数分
            loadMaterial(mat.c_str(), &mMaterials);
        }
        //頂点
        if (strip == "v") {
            mNumVert++;
        }
        //法線
        if (strip == "vn") {
            mNumNormal++;
        }
        //テクスチャー座標
        if (strip == "vt") {
            mNumTex++;
        }
        //フェイス(ポリゴン)
        if (strip == "f") {
            mNumFace++;
        }
    }
    //テクスチャー座標 > 頂点数がありえる。その場合、頂点を増やす必要がある
    if (mNumTex > mNumVert) {
        mNumVert = mNumTex;
    }

    return true;
}

bool Mesh::loadMaterial(const char* fileName, std::vector<Material> * material) {
    //マテリアルファイルを開いて内容を読み込む
    std::ifstream ifs(fileName, std::ios::in);
    if (ifs.fail()) {
        MSG(L"mtlファイルが存在しません");
        return false;
    }

    //マテリアル数を調べる
    mNumMaterial = 0;
    std::string line;
    while (!ifs.eof()) {
        //キーワード読み込み
        std::getline(ifs, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        auto strip = stringStrip(line, ' ');

        //マテリアル名
        if (strip == "newmtl") {
            mNumMaterial++;
        }
    }
    material->resize(mNumMaterial);
    std::vector<Material> materials(mNumMaterial);

    //本読み込み
    ifs.clear();
    ifs.seekg(0, std::ios_base::beg);
    Vector4 v(0.f, 0.f, 0.f, 1.f);
    int matCount = -1;

    while (!ifs.eof()) {
        //キーワード読み込み
        std::getline(ifs, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        auto strip = stringStrip(line, ' ');

        //マテリアル名
        if (strip == "newmtl") {
            matCount++;
            materials[matCount].matName = line.substr(7); //「newmtl 」の文字数分
        }
        //Ka アンビエント
        if (strip == "Ka") {
            auto sub = line.substr(3); //「Ka 」の文字数分
            sscanf_s(sub.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
            materials[matCount].Ka = v;
        }
        //Kd ディフューズ
        if (strip == "Kd") {
            auto sub = line.substr(3); //「Kd 」の文字数分
            auto i = sscanf_s(sub.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
            materials[matCount].Kd = v;
        }
        //Ks スペキュラー
        if (strip == "Ks") {
            auto sub = line.substr(3); //「Ks 」の文字数分
            sscanf_s(sub.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
            materials[matCount].Ks = v;
        }
        //map_Kd テクスチャー
        if (strip == "map_Kd") {
            materials[matCount].textureName = line.substr(7); //「map_Kd 」の文字数分
            //テクスチャーを作成
            if (FAILED(D3DX11CreateShaderResourceViewFromFileA(mRenderer->device(), materials[matCount].textureName.c_str(), nullptr, nullptr, &materials[matCount].texture, nullptr))) {
                MSG(L"Meshのテクスチャが存在しません");
                return false;
            }
            D3D11_SAMPLER_DESC sd;
            ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
            sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
            sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
            sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
            mRenderer->device()->CreateSamplerState(&sd, &materials[matCount].sampleLinear);
        }
    }

    material->swap(materials);

    return true;
}

void Mesh::rendererMesh() const {
    //使用するシェーダーの登録
    mShader->setVSShader();
    mShader->setPSShader();
    //このコンスタントバッファーを使うシェーダーの登録
    mShader->setVSConstantBuffers(0);
    mShader->setPSConstantBuffers(0);
    //頂点インプットレイアウトをセット
    mShader->setInputLayout();

    //シェーダーのコンスタントバッファーに各種データを渡す
    D3D11_MAPPED_SUBRESOURCE pData;
    if (SUCCEEDED(mRenderer->deviceContext()->Map(mShader->getConstantBuffer(0)->buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
        MeshShaderConstantBuffer0 cb;
        //ワールド行列を渡す
        cb.world = mTransform->getWorldTransform();
        cb.world.transpose();
        //ワールド、カメラ、射影行列を渡す
        cb.WVP = mTransform->getWorldTransform() /** Singleton<Camera>::instance().getView() * Singleton<Camera>::instance().getProjection()*/;
        cb.WVP.transpose();
        //ライトの方向を渡す
        cb.lightDir = Vector4(1.f, -1.f, 1.f, 0.0f);
        //視点位置を渡す
        //sg.eye = Vector4(Singleton<Camera>::instance().getPosition(), 0);

        memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(cb));
        mRenderer->deviceContext()->Unmap(mShader->getConstantBuffer(0)->buffer(), 0);
    }

    //バーテックスバッファーをセット
    VertexStreamDesc stream;
    stream.sharedBuffer = mVertexBuffer;
    stream.offset = 0;
    stream.stride = sizeof(MeshVertex);
    mRenderer->setVertexBuffer(&stream);

    //マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    for (unsigned i = 0; i < mNumMaterial; i++) {
        //使用されていないマテリアル対策
        if (mMaterials[i].numFace == 0) {
            continue;
        }
        //インデックスバッファーをセット
        stream.stride = sizeof(int);
        stream.offset = 0;
        mRenderer->setIndexBuffer(mIndexBuffers[i]);

        //マテリアルの各要素をエフェクト(シェーダー)に渡す
        D3D11_MAPPED_SUBRESOURCE pData;
        if (SUCCEEDED(mRenderer->deviceContext()->Map(mShader->getConstantBuffer(1)->buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
            MeshShaderConstantBuffer1 sg;
            sg.ambient = mMaterials[i].Ka; //アンビエントををシェーダーに渡す
            sg.diffuse = mMaterials[i].Kd; //ディフューズカラーをシェーダーに渡す
            sg.specular = mMaterials[i].Ks; //スペキュラーをシェーダーに渡す

            //このコンスタントバッファーを使うシェーダーの登録
            mShader->setVSConstantBuffers(1);
            mShader->setVSConstantBuffers(1);

            //テクスチャーをシェーダーに渡す
            if (mMaterials[i].texture) {
                mRenderer->deviceContext()->PSSetShaderResources(0, 1, &mMaterials[i].texture);
                mRenderer->deviceContext()->PSSetSamplers(0, 1, &mMaterials[i].sampleLinear);
                sg.texture.x = 1;
            } else {
                sg.texture.x = 0;
            }

            memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(sg));
            mRenderer->deviceContext()->Unmap(mShader->getConstantBuffer(1)->buffer(), 0);
        }
        //プリミティブをレンダリング
        mRenderer->drawIndexed(mMaterials[i].numFace * 3);
    }
}

std::string Mesh::stringStrip(const std::string & string, const char delimiter) {
    std::string temp = "";
    for (const auto& s : string) {
        if (s != delimiter) {
            temp += s;
        } else {
            break;
        }
    }
    return temp;
}

MeshManager* Mesh::mMeshManager = nullptr;
