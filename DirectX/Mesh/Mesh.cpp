#include "Mesh.h"
#include "MeshManager.h"
#include "../Actor/DirectionalLight.h"
#include "../Actor/Transform3D.h"
#include "../Camera/Camera.h"
#include "../Device/Renderer.h"
#include "../Shader/Shader.h"
#include "../Sprite/Texture.h"
#include "../System/Buffer.h"
#include "../System/GBuffer.h"
#include "../System/InputElement.h"
#include "../System/VertexArray.h"
#include <fstream>
#include <string>
#include <stdio.h>

Mesh::Mesh(std::shared_ptr<Renderer> renderer, const char* fileName) :
    mTransform(nullptr),
    mShader(renderer->createShader("GBuffer.hlsl")),
    mMaterials(0),
    mVertexArray(std::make_unique<VertexArray>(renderer)),
    mState(MeshState::ACTIVE) {
    if (!loadMesh(renderer, fileName)) {
        MSG(L"メッシュ作成失敗");
        return;
    }

    //メッシュ用コンスタントバッファの作成
    mShader->createConstantBuffer(renderer, sizeof(MeshShaderConstantBuffer0), 0);
    //mShader->createConstantBuffer(renderer, sizeof(MeshShaderConstantBuffer1), 1);

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

void Mesh::createSphere(std::shared_ptr<Sphere>* sphere) const {
    //バウンディングスフィア作成
    auto verts = mVertexArray->getVertices();
    float min = Math::infinity;
    float max = Math::negInfinity;
    Vector3 minVec3 = Vector3::one * Math::infinity;
    Vector3 maxVec3 = Vector3::one * Math::negInfinity;
    for (size_t i = 0; i < mVertexArray->getNumVerts(); i++) {
        //以下半径
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

        //以下中心
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

void Mesh::drawAll(std::list<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera) {
    //プリミティブ・トポロジーをセット
    renderer->setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_LIST);

    //各テクスチャ上にレンダリング
    renderToTexture(renderer);

    for (const auto& mesh : meshes) {
        if (!mesh->getActive() || mesh->isDead()) {
            continue;
        }

        renderer->setRasterizerStateFront();
        mesh->renderMesh(renderer, camera);
        renderer->setRasterizerStateBack();
        mesh->renderMesh(renderer, camera);
    }

    //各テクスチャを参照してレンダリング
    renderFromTexture(renderer, camera);
}

void Mesh::setTransform(std::shared_ptr<Transform3D> transform) {
    mTransform = transform;
}

void Mesh::destroy() {
    mState = MeshState::DEAD;
}

void Mesh::setActive(bool value) {
    mState = (value) ? MeshState::ACTIVE : MeshState::NON_ACTIVE;
}

bool Mesh::getActive() const {
    return mState == MeshState::ACTIVE;
}

bool Mesh::isDead() const {
    return mState == MeshState::DEAD;
}

void Mesh::setMeshManager(MeshManager* manager) {
    mMeshManager = manager;
}

bool Mesh::loadMesh(std::shared_ptr<Renderer> renderer, const char* fileName) {
    //事前に頂点数などを調べる
    if (!tempLoad(renderer, fileName)) {
        MSG(L"Meshファイルの事前読み込み失敗");
        return false;
    }

    //サイズ変更
    Vector3* vertices = new Vector3[mVertexArray->getNumVerts()];
    Vector3* normals = new Vector3[mVertexArray->getNumNormal()];
    Vector2* textures = new Vector2[mVertexArray->getNumTex()];

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
            vertices[vCount].x = -x;
            vertices[vCount].y = y;
            vertices[vCount].z = z;
            vCount++;
        }

        //法線読み込み
        if (strip == "vn") {
            auto sub = line.substr(3); //「vn 」の文字数分
            sscanf_s(sub.c_str(), "%f %f %f", &x, &y, &z);
            normals[vnCount].x = -x;
            normals[vnCount].y = y;
            normals[vnCount].z = z;
            vnCount++;
        }

        //テクスチャー座標 読み込み
        if (strip == "vt") {
            auto sub = line.substr(3); //「vt 」の文字数分
            sscanf_s(sub.c_str(), "%f %f", &x, &y);
            textures[vtCount].x = x;
            textures[vtCount].y = 1 - y; //OBJファイルはY成分が逆なので合わせる
            vtCount++;
        }
    }

    //マテリアルの数だけインデックスバッファーを作成
    mVertexArray->resizeIndexBuffer(mMaterials.size());
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

    for (unsigned i = 0; i < mMaterials.size(); i++) {
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
                matFlag = (mMaterials[i]->matName == mat);
            }

            if (strip == "f" && matFlag) {
                auto sub = line.substr(2); //「f 」の文字数分
                if (mMaterials[i]->texture) { //テクスチャーありサーフェイス
                    sscanf_s(sub.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
                } else { //テクスチャー無しサーフェイス
                    sscanf_s(sub.c_str(), "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
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
        mVertexArray->createIndexBuffer(i, fCount, faceBuffer);

        mMaterials[i]->numFace = fCount;
    }

    delete[] normals;
    delete[] textures;
    delete[] faceBuffer;

    //バーテックスバッファーを作成
    mVertexArray->createVertexBuffer(sizeof(MeshVertex), meshVertices);

    delete[] meshVertices;

    return true;
}

bool Mesh::tempLoad(std::shared_ptr<Renderer> renderer, const char* fileName) {
    //OBJファイルを開いて内容を読み込む
    setOBJDirectory();
    std::ifstream ifs(fileName, std::ios::in);
    if (ifs.fail()) {
        MSG(L"OBJファイルが存在しません");
        return false;
    }

    unsigned numVert = 0;
    unsigned numNormal = 0;
    unsigned numTex = 0;
    unsigned numFace = 0;
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
            loadMaterial(renderer, mat.c_str(), &mMaterials);
        }
        //頂点
        if (strip == "v") {
            numVert++;
        }
        //法線
        if (strip == "vn") {
            numNormal++;
        }
        //テクスチャー座標
        if (strip == "vt") {
            numTex++;
        }
        //フェイス(ポリゴン)
        if (strip == "f") {
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

bool Mesh::loadMaterial(std::shared_ptr<Renderer> renderer, const char* fileName, std::vector<std::unique_ptr<Material>>* materials) {
    //マテリアルファイルを開いて内容を読み込む
    std::ifstream ifs(fileName, std::ios::in);
    if (ifs.fail()) {
        MSG(L"mtlファイルが存在しません");
        return false;
    }

    //マテリアル数を調べる
    if (!materials->empty()) {
        materials->clear();
    }

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
            materials->emplace_back(std::make_unique<Material>());
        }
    }

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
            (*materials)[matCount]->matName = line.substr(7); //「newmtl 」の文字数分
        }
        //Ka アンビエント
        if (strip == "Ka") {
            auto sub = line.substr(3); //「Ka 」の文字数分
            sscanf_s(sub.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
            (*materials)[matCount]->Ka = v;
        }
        //Kd ディフューズ
        if (strip == "Kd") {
            auto sub = line.substr(3); //「Kd 」の文字数分
            auto i = sscanf_s(sub.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
            (*materials)[matCount]->Kd = v;
        }
        //Ks スペキュラー
        if (strip == "Ks") {
            auto sub = line.substr(3); //「Ks 」の文字数分
            sscanf_s(sub.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
            (*materials)[matCount]->Ks = v;
        }
        //map_Kd テクスチャー
        if (strip == "map_Kd") {
            (*materials)[matCount]->textureName = line.substr(7); //「map_Kd 」の文字数分
            //テクスチャーを作成
            (*materials)[matCount]->texture = renderer->createTexture((*materials)[matCount]->textureName.c_str(), false);
        }
    }

    return true;
}

void Mesh::renderMesh(std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera) const {
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
    if (mShader->map(&pData, 0)) {
        MeshShaderConstantBuffer0 cb;
        //ワールド行列を渡す
        cb.world = mTransform->getWorldTransform();
        cb.world.transpose();
        //ワールド、カメラ、射影行列を渡す
        cb.WVP = mTransform->getWorldTransform() * camera->getView() * camera->getProjection();
        cb.WVP.transpose();
        //ライトの方向を渡す
        //cb.lightDir = DirectionalLight::direction;
        //cb.lightPos = SpotLight::position;
        //cb.lightDir = SpotLight::rot;
        //cb.lightDir.transpose();
        //視点位置を渡す
        //cb.eye = camera->getPosition();

        memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(cb));
        mShader->unmap(0);
    }

    //バーテックスバッファーをセット
    mVertexArray->setVertexBuffer(sizeof(MeshVertex));

    //このコンスタントバッファーを使うシェーダーの登録
    //mShader->setVSConstantBuffers(1);
    //mShader->setPSConstantBuffers(1);

    //マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    for (size_t i = 0; i < mMaterials.size(); i++) {
        //使用されていないマテリアル対策
        if (mMaterials[i]->numFace == 0) {
            continue;
        }
        //インデックスバッファーをセット
        mVertexArray->setIndexBuffer(i);

        //マテリアルの各要素をエフェクト(シェーダー)に渡す
        //D3D11_MAPPED_SUBRESOURCE pData;
        //if (SUCCEEDED(renderer->deviceContext()->Map(mShader->getConstantBuffer(1)->buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
        //    MeshShaderConstantBuffer1 cb;
        //    cb.ambient = mMaterials[i]->Ka; //アンビエントををシェーダーに渡す
        //    cb.diffuse = mMaterials[i]->Kd; //ディフューズカラーをシェーダーに渡す
        //    cb.specular = mMaterials[i]->Ks; //スペキュラーをシェーダーに渡す

        //    //テクスチャーをシェーダーに渡す
        //    if (mMaterials[i]->texture) {
        //        renderer->deviceContext()->PSSetShaderResources(0, 1, &mMaterials[i]->texture);
        //        renderer->deviceContext()->PSSetSamplers(0, 1, &mMaterials[i]->sampleLinear);
        //        cb.texture = 1;
        //    } else {
        //        cb.texture = 0;
        //    }

        //    memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(cb));
        //    renderer->deviceContext()->Unmap(mShader->getConstantBuffer(1)->buffer(), 0);
        //}

        if (mMaterials[i]->texture) {
            mMaterials[i]->texture->setPSTextures();
            mMaterials[i]->texture->setPSSamplers();
        }
        //プリミティブをレンダリング
        renderer->drawIndexed(mMaterials[i]->numFace * 3);
    }
}

void Mesh::renderToTexture(std::shared_ptr<Renderer> renderer) {
    //各テクスチャをレンダーターゲットに設定
    static constexpr unsigned numGBuffer = static_cast<unsigned>(GBuffer::Type::NUM_GBUFFER_TEXTURES);
    ID3D11RenderTargetView* views[numGBuffer];
    for (size_t i = 0; i < numGBuffer; i++) {
        views[i] = renderer->getGBuffer()->getRenderTarget(i);
    }
    renderer->setRenderTargets(views, numGBuffer);
    //クリア
    for (size_t i = 0; i < numGBuffer; i++) {
        renderer->clearRenderTarget(views[i]);
    }
    renderer->clearDepthStencilView();
}

void Mesh::renderFromTexture(std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera) {
    //レンダーターゲットを通常に戻す
    renderer->setDefaultRenderTarget();
    //クリア
    renderer->clear();

    //使用するシェーダーは、テクスチャーを参照するシェーダー
    renderer->getGBuffer()->shader()->setVSShader();
    renderer->getGBuffer()->shader()->setPSShader();
    //1パス目で作成したテクスチャー3枚をセット
    static constexpr unsigned numGBuffer = static_cast<unsigned>(GBuffer::Type::NUM_GBUFFER_TEXTURES);
    for (size_t i = 0; i < numGBuffer; i++) {
        auto sr = renderer->getGBuffer()->getShaderResource(i);
        renderer->deviceContext()->PSSetShaderResources(i, 1, &sr);
    }

    D3D11_MAPPED_SUBRESOURCE pData;
    if (renderer->getGBuffer()->shader()->map(&pData)) {
        GBufferShaderConstantBuffer cb;
        //ライトの方向を渡す
        //cb.lightDir = Vector3::up;
        cb.lightDir = DirectionalLight::direction;
        //視点位置を渡す
        cb.eye = camera->getPosition();

        memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(cb));
        renderer->getGBuffer()->shader()->unmap();
    }
    //スクリーンサイズのポリゴンをレンダー
    renderer->setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_STRIP);
    //バーテックスバッファーをセット
    VertexStreamDesc stream;
    stream.sharedBuffer = renderer->getGBuffer()->vertexBuffer();
    stream.offset = 0;
    stream.stride = sizeof(MeshVertex);
    renderer->setVertexBuffer(&stream);

    renderer->draw(4);
}

std::string Mesh::stringStrip(const std::string& string, const char delimiter) {
    //デリミタが見つかるまでの文字を返す
    return string.substr(0, string.find_first_of(delimiter));
}

MeshManager* Mesh::mMeshManager = nullptr;
