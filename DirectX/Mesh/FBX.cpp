#include "FBX.h"
#include "../System/Game.h"
#include "../System/VertexArray.h"

FBX::FBX() :
    mManager(nullptr),
    mVertices(nullptr),
    mVertexArray(std::make_shared<VertexArray>()) {
}

FBX::~FBX() {
    //削除されてると思うけど
    SAFE_DELETE_ARRAY(mVertices);
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

    mVertexArray->createVertexBuffer(sizeof(Vertex), mVertices);
    SAFE_DELETE_ARRAY(mVertices);
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
                getVertex(mesh);
                getNormals(mesh);
                getUV(mesh);
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

void FBX::getVertex(FbxMesh* mesh) {
    //頂点数
    int vertexNum = mesh->GetControlPointsCount();
    mVertexArray->setNumVerts(vertexNum);
    //頂点座標配列
    FbxVector4* src = mesh->GetControlPoints();

    mVertices = new Vertex[vertexNum];
    for (size_t i = 0; i < vertexNum; i++) {
        mVertices[i].pos.x = -src[i][0];
        mVertices[i].pos.y = src[i][1];
        mVertices[i].pos.z = src[i][2];
        mVertices[i].pos.w = src[i][3];
    }
}

void FBX::searchIndex(FbxMesh* mesh) {
    //総ポリゴン数
    int polyNum = mesh->GetPolygonCount();
    mVertexArray->setNumFace(polyNum);
    //インデックス数
    auto count = mesh->GetPolygonVertexCount();
    //インデックス配列
    auto indices = mesh->GetPolygonVertices();

    mVertexArray->resizeIndexBuffer(1);
    mVertexArray->createIndexBuffer(0, count, indices);
}

void FBX::getNormals(FbxMesh* mesh) {
    FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();
    if (!normalElement) {
        return;
    }

    if (normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
        for (size_t i = 0; i < mesh->GetControlPointsCount(); i++) {
            int normalIndex = 0;
            if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
                normalIndex = i;
            }
            if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
                normalIndex = normalElement->GetIndexArray().GetAt(i);
            }

            FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);
            mVertices[normalIndex].normal.x = -normal[0];
            mVertices[normalIndex].normal.y = normal[1];
            mVertices[normalIndex].normal.z = normal[2];
            mVertices[normalIndex].normal.w = normal[3];
        }
    } else if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
        int indexByPolygonVertex = 0;
        for (size_t i = 0; i < mesh->GetPolygonCount(); i++) {
            int polygonSize = mesh->GetPolygonSize(i);
            for (size_t j = 0; j < polygonSize; j++) {
                int normalIndex = 0;
                if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
                    normalIndex = indexByPolygonVertex;
                }
                if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
                    normalIndex = normalElement->GetIndexArray().GetAt(indexByPolygonVertex);
                }

                FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);
                mVertices[normalIndex].normal.x = -normal[0];
                mVertices[normalIndex].normal.y = normal[1];
                mVertices[normalIndex].normal.z = normal[2];
                mVertices[normalIndex].normal.w = normal[3];

                indexByPolygonVertex++;
            }
        }
    }

    //FbxVector4 normal;
    //int* indices = mesh->GetPolygonVertices();
    //for (int i = 0; i < mVertexArray->getNumFace(); i++) {
    //    int startIndex = mesh->GetPolygonVertexIndex(i);

    //    int vertIndex0 = indices[startIndex];
    //    int vertIndex1 = indices[startIndex + 1];
    //    int vertIndex2 = indices[startIndex + 2];

    //    if (vertIndex0 < 0) {
    //        continue;
    //    }

    //    mesh->GetPolygonVertexNormal(i, 0, normal);
    //    mVertices[vertIndex0].normal.x = -normal[0];
    //    mVertices[vertIndex0].normal.y = normal[1];
    //    mVertices[vertIndex0].normal.z = normal[2];
    //    mVertices[vertIndex0].normal.w = normal[3];

    //    mesh->GetPolygonVertexNormal(i, 1, normal);
    //    mVertices[vertIndex1].normal.x = -normal[0];
    //    mVertices[vertIndex1].normal.y = normal[1];
    //    mVertices[vertIndex1].normal.z = normal[2];
    //    mVertices[vertIndex1].normal.w = normal[3];

    //    mesh->GetPolygonVertexNormal(i, 2, normal);
    //    mVertices[vertIndex2].normal.x = -normal[0];
    //    mVertices[vertIndex2].normal.y = normal[1];
    //    mVertices[vertIndex2].normal.z = normal[2];
    //    mVertices[vertIndex2].normal.w = normal[3];
    //}
}

void FBX::getUV(FbxMesh* mesh) {
    FbxStringList uvSetNameList;
    mesh->GetUVSetNames(uvSetNameList);

    //すべてのUVセットを反復処理する
    for (int uvSetIndex = 0; uvSetIndex < uvSetNameList.GetCount(); uvSetIndex++) {
        //uvSetIndex-番目のUVセットを取得
        const char* uvSetName = uvSetNameList.GetStringAt(uvSetIndex);
        const FbxGeometryElementUV* uvElement = mesh->GetElementUV(uvSetName);
        if (!uvElement) {
            continue;
        }

        //マッピングモードeByPolygonVertexおよびeByControlPointのみをサポート
        if (uvElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex && uvElement->GetMappingMode() != FbxGeometryElement::eByControlPoint) {
            return;
        }

        //インデックス配列。uvデータを参照するインデックスを保持します
        const bool useIndex = uvElement->GetReferenceMode() != FbxGeometryElement::eDirect;
        const int indexCount = (useIndex) ? uvElement->GetIndexArray().GetCount() : 0;

        //ポリゴンごとにデータを反復処理する
        const int polyCount = mesh->GetPolygonCount();

        if (uvElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
            for (int polyIndex = 0; polyIndex < polyCount; polyIndex++) {
                //MakePolyに渡す必要がある最大インデックス配列を作成します
                const int polySize = mesh->GetPolygonSize(polyIndex);
                for (int vertIndex = 0; vertIndex < polySize; vertIndex++) {
                    //頂点配列の現在の頂点のインデックスを取得します
                    int polyVertIndex = mesh->GetPolygonVertex(polyIndex, vertIndex);

                    //UVインデックスは参照モードに依存します
                    int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyVertIndex) : polyVertIndex;

                    FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
                    mVertices[uvIndex].normal.x = uv[0];
                    mVertices[uvIndex].normal.y = 1.f - uv[1];
                }
            }
        } else if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
            int polyIndexCounter = 0;
            for (int polyIndex = 0; polyIndex < polyCount; polyIndex++) {
                //MakePolyに渡す必要がある最大インデックス配列を作成します
                const int polySize = mesh->GetPolygonSize(polyIndex);
                for (int vertIndex = 0; vertIndex < polySize; vertIndex++) {
                    if (polyIndexCounter < indexCount) {
                        //UVインデックスは参照モードに依存します
                        int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyIndexCounter) : polyIndexCounter;

                        FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
                        mVertices[uvIndex].uv.x = uv[0];
                        mVertices[uvIndex].uv.y = 1.f - uv[1];

                        polyIndexCounter++;
                    }
                }
            }
        }
    }
}
