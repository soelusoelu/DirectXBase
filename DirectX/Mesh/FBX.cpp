#include "FBX.h"
#include "../System/Game.h"
#include "../System/VertexArray.h"

FBX::FBX() :
    mManager(nullptr),
    mVertexArray(std::make_unique<VertexArray>()) {
}

FBX::~FBX() {
    //マネージャー解放
    if (mManager) {
        mManager->Destroy();
    }
}

void FBX::initialize() {
    //マネージャーを生成
    mManager = FbxManager::Create();

    //IOSettingを生成
    FbxIOSettings* ioSettings = FbxIOSettings::Create(mManager, IOSROOT);

    //Importerを生成
    FbxImporter* importer = FbxImporter::Create(mManager, "");
    if (!importer->Initialize("SD_unitychan_humanoid.fbx", -1, mManager->GetIOSettings())) {
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
        enumNodeNamesAndAttributes(root, 0);
    }
}

void FBX::enumNodeNamesAndAttributes(FbxNode* node, int indent) {
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
                searchVertex(mesh);
                break;
            default:
                break;
            }
        }
    }

    int childCount = node->GetChildCount();
    for (int i = 0; i < childCount; ++i) {
        enumNodeNamesAndAttributes(node->GetChild(i), indent + 1);
    }
}

void FBX::searchIndex(FbxMesh* mesh) {
    //総ポリゴン数
    int polyNum = mesh->GetPolygonCount();
    //インデックス数
    auto count = mesh->GetPolygonVertexCount();
    //インデックス配列
    auto indices = mesh->GetPolygonVertices();

    //for (size_t i = 0; i < polyNum; i++) {
    //    for (size_t j = 0; j < 3; j++) {
    //        int index = mesh->GetPolygonVertex(i, j);
    //        std::cout << "index[" << i + j << "] : " << index << std::endl;
    //    }
    //}

    mVertexArray->createIndexBuffer(0, count, indices);
}

void FBX::searchVertex(FbxMesh* mesh) {
    //頂点数
    int vertexNum = mesh->GetControlPointsCount();
    mVertexArray->setNumVerts(vertexNum);
    //頂点座標配列
    FbxVector4* src = mesh->GetControlPoints();

    Vertex* vertices = new Vertex[vertexNum];
    for (size_t i = 0; i < vertexNum; i++) {
        vertices[i].pos.x = src[i][0];
        vertices[i].pos.y = src[i][1];
        vertices[i].pos.z = src[i][2];
        vertices[i].pos.w = src[i][3];
    }

    mVertexArray->createVertexBuffer(sizeof(Vertex), vertices);

    SAFE_DELETE_ARRAY(vertices);
}
