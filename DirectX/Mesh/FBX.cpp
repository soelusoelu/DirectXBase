#include "FBX.h"
#include "../System/Game.h"
#include "../System/VertexArray.h"

FBX::FBX() :
    mManager(nullptr),
    mVertexArray(std::make_shared<VertexArray>()) {
}

FBX::~FBX() {
    //マネージャー解放
    if (mManager) {
        mManager->Destroy();
    }
}

void FBX::create(const std::string& fileName) {
    setOBJDirectory();

    //マネージャーを生成
    mManager = FbxManager::Create();

    //IOSettingを生成
    FbxIOSettings* ioSettings = FbxIOSettings::Create(mManager, IOSROOT);
    mManager->SetIOSettings(ioSettings);

    //Importerを生成
    FbxImporter* importer = FbxImporter::Create(mManager, "");
    if (!importer->Initialize(fileName.c_str(), -1, mManager->GetIOSettings())) {
        //インポートエラー
        return;
    }

    //SceneオブジェクトにFBXファイル内の情報を流し込む
    FbxScene* scene = FbxScene::Create(mManager, "scene");
    importer->Import(scene);
    importer->Destroy(); //シーンを流し込んだらImporterは解放していい

    //四角ポリゴンを三角ポリゴンに変換
    FbxGeometryConverter geometryConverter(mManager);
    geometryConverter.Triangulate(scene, true);

    //Scene解析
    FbxNode* root = scene->GetRootNode();
    if (root) {
        perse(root, 0);
    }
}

std::shared_ptr<VertexArray> FBX::getVertexArray() const {
    return mVertexArray;
}

void FBX::perse(FbxNode* node, int indent) {
    int attrCount = node->GetNodeAttributeCount();

    for (size_t i = 0; i < attrCount; i++) {
        FbxNodeAttribute* attr = node->GetNodeAttributeByIndex(i);
        FbxNodeAttribute::EType type = attr->GetAttributeType();
        if (i + 1 == attrCount) {
            FbxMesh* mesh = nullptr;
            switch (type) {
            case fbxsdk::FbxNodeAttribute::eMesh:
                mesh = node->GetMesh();
                searchIndex(mesh);
                createVertex(mesh);
                break;
            default:
                break;
            }
        }
    }

    int childCount = node->GetChildCount();
    for (int i = 0; i < childCount; ++i) {
        perse(node->GetChild(i), indent + 1);
    }
}

void FBX::createVertex(FbxMesh* mesh) {
    //総ポリゴン数
    int polyNum = mesh->GetPolygonCount();
    mVertexArray->setNumFace(polyNum);
    //インデックス数
    auto count = mesh->GetPolygonVertexCount();
    //インデックス配列
    auto indices = mesh->GetPolygonVertices();

    mVertexArray->resizeIndexBuffer(1);
    mVertexArray->createIndexBuffer(0, count, indices);

    //頂点数
    int vertexNum = mesh->GetControlPointsCount();
    mVertexArray->setNumVerts(vertexNum);
    //頂点座標配列
    FbxVector4* src = mesh->GetControlPoints();

    Vertex* vertices = new Vertex[vertexNum];
    for (size_t i = 0; i < vertexNum; i++) {
        vertices[i].pos.x = -src[i][0];
        vertices[i].pos.y = src[i][1];
        vertices[i].pos.z = src[i][2];
        vertices[i].pos.w = src[i][3];
    }

    //法線
    //int layerNum = mesh->GetLayerCount();
    //Vector3* normalBuffer = nullptr;
    //for (size_t i = 0; i < layerNum; i++) {
    //    FbxLayer* layer = mesh->GetLayer(i);
    //    FbxLayerElementNormal* elementNormal = layer->GetNormals();
    //    if (!elementNormal) {
    //        continue;
    //    }

    //    //法線の数・インデックス
    //    int normalNum = elementNormal->GetDirectArray().GetCount();
    //    int indexNum = elementNormal->GetIndexArray().GetCount();
    //    mVertexArray->setNumNormal(normalNum);

    //    normalBuffer = new Vector3[normalNum];

    //    //マッピングモード・リファレンスモード取得
    //    FbxLayerElement::EMappingMode mappingMode = elementNormal->GetMappingMode();
    //    FbxLayerElement::EReferenceMode referenceMode = elementNormal->GetReferenceMode();

    //    if (mappingMode == FbxLayerElement::eByPolygonVertex) {
    //        if (referenceMode == FbxLayerElement::eDirect) {
    //            //直接取得
    //            for (size_t i = 0; i < normalNum; i++) {
    //                normalBuffer[i].x = static_cast<float>(elementNormal->GetDirectArray().GetAt(i)[0]);
    //                normalBuffer[i].y = static_cast<float>(elementNormal->GetDirectArray().GetAt(i)[1]);
    //                normalBuffer[i].z = static_cast<float>(elementNormal->GetDirectArray().GetAt(i)[2]);
    //            }
    //        }
    //    } else if (mappingMode == FbxLayerElement::eByControlPoint) {
    //        if (referenceMode == FbxLayerElement::eDirect) {
    //            //直接取得
    //            for (size_t i = 0; i < normalNum; i++) {
    //                normalBuffer[i].x = static_cast<float>(elementNormal->GetDirectArray().GetAt(i)[0]);
    //                normalBuffer[i].y = static_cast<float>(elementNormal->GetDirectArray().GetAt(i)[1]);
    //                normalBuffer[i].z = static_cast<float>(elementNormal->GetDirectArray().GetAt(i)[2]);
    //            }
    //        }
    //    }
    //}
    FbxVector4 normal;
    for (int i = 0; i < mVertexArray->getNumFace(); i++) {
        int startIndex = mesh->GetPolygonVertexIndex(i);

        int vertIndex0 = indices[startIndex];
        int vertIndex1 = indices[startIndex + 1];
        int vertIndex2 = indices[startIndex + 2];

        if (vertIndex0 < 0) {
            continue;
        }

        mesh->GetPolygonVertexNormal(i, 0, normal);
        vertices[vertIndex0].normal.x = -normal[0];
        vertices[vertIndex0].normal.y = normal[1];
        vertices[vertIndex0].normal.z = normal[2];
        vertices[vertIndex0].normal.w = normal[3];

        mesh->GetPolygonVertexNormal(i, 1, normal);
        vertices[vertIndex1].normal.x = -normal[0];
        vertices[vertIndex1].normal.y = normal[1];
        vertices[vertIndex1].normal.z = normal[2];
        vertices[vertIndex1].normal.w = normal[3];

        mesh->GetPolygonVertexNormal(i, 2, normal);
        vertices[vertIndex2].normal.x = -normal[0];
        vertices[vertIndex2].normal.y = normal[1];
        vertices[vertIndex2].normal.z = normal[2];
        vertices[vertIndex2].normal.w = normal[3];
    }

    //テクスチャ座標読み込み
    auto uvCount = mesh->GetTextureUVCount();
    mVertexArray->setNumTex(uvCount);
    FbxLayerElementUV* elementUV = mesh->GetLayer(0)->GetUVs();
    for (int i = 0; i < uvCount; i++) {
        FbxVector2 uv;
        uv = elementUV->GetDirectArray().GetAt(i);
        //vertices[i].uv.x = uv.;
        //vertices[i].uv.y = 1.f - uv.GetAt(1);
    }

    mVertexArray->createVertexBuffer(sizeof(Vertex), vertices);

    SAFE_DELETE_ARRAY(vertices);
    //SAFE_DELETE_ARRAY(normalBuffer);
}

void FBX::searchIndex(FbxMesh* mesh) {
    //総ポリゴン数
    //int polyNum = mesh->GetPolygonCount();
    //mVertexArray->setNumFace(polyNum);
    ////インデックス数
    //auto count = mesh->GetPolygonVertexCount();
    ////インデックス配列
    //auto indices = mesh->GetPolygonVertices();

    //mVertexArray->resizeIndexBuffer(1);
    //mVertexArray->createIndexBuffer(0, count, indices);
}
