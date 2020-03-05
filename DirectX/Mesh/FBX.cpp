#include "FBX.h"
#include "Material.h"
#include "VertexArray.h"
#include "../Device/AssetsManager.h"
#include "../System/Game.h"

FBX::FBX() :
    mManager(nullptr),
    mVertices(nullptr),
    mVertexArray(std::make_shared<VertexArray>()) {
}

FBX::~FBX() {
    SAFE_DELETE_ARRAY(mVertices);
    //マネージャー解放
    if (mManager) {
        mManager->Destroy();
    }
}

void FBX::perse(std::shared_ptr<AssetsManager> assetsManager, const std::string& fileName) {
    setModelDirectory();

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
        perse(assetsManager, root, 0);
    }

    mVertexArray->createVertexBuffer(sizeof(MeshVertex), mVertices);
}

std::shared_ptr<Material> FBX::getMaterial(unsigned index) const {
    return mMaterials[index];
}

std::shared_ptr<VertexArray> FBX::getVertexArray() const {
    return mVertexArray;
}

size_t FBX::getNumMaterial() const {
    return mMaterials.size();
}

void FBX::createSphere(std::shared_ptr<Sphere>* sphere) const {
    //バウンディングスフィア作成
    float min = Math::infinity;
    float max = Math::negInfinity;
    Vector3 minVec3 = Vector3::one * Math::infinity;
    Vector3 maxVec3 = Vector3::one * Math::negInfinity;
    for (size_t i = 0; i < mVertexArray->getNumVerts(); i++) {
        //半径
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

        //中心
        if (mVertices[i].pos.x < minVec3.x) {
            minVec3.x = mVertices[i].pos.x;
        }
        if (mVertices[i].pos.x > maxVec3.x) {
            maxVec3.x = mVertices[i].pos.x;
        }
        if (mVertices[i].pos.y < minVec3.y) {
            minVec3.y = mVertices[i].pos.y;
        }
        if (mVertices[i].pos.y > maxVec3.y) {
            maxVec3.y = mVertices[i].pos.y;
        }
        if (mVertices[i].pos.z < minVec3.z) {
            minVec3.z = mVertices[i].pos.z;
        }
        if (mVertices[i].pos.z > maxVec3.z) {
            maxVec3.z = mVertices[i].pos.z;
        }
    }
    float r = (max - min) / 2.f;
    (*sphere)->radius = r;
    auto c = (maxVec3 + minVec3) / 2.f;
    (*sphere)->center = c;
}

void FBX::perse(std::shared_ptr<AssetsManager> assets, FbxNode* node, int indent) {
    int attrCount = node->GetNodeAttributeCount();

    for (size_t i = 0; i < attrCount; i++) {
        FbxNodeAttribute* attr = node->GetNodeAttributeByIndex(i);
        FbxNodeAttribute::EType type = attr->GetAttributeType();
        if (i + 1 == attrCount) {
            FbxMesh* mesh = nullptr;
            switch (type) {
            case fbxsdk::FbxNodeAttribute::eMesh:
                mesh = node->GetMesh();
                getIndex(mesh);
                getVertex(mesh);
                getNormals(mesh);
                getUV(mesh);
                getMaterial(assets, mesh);
                break;
            default:
                break;
            }
        }
    }

    int childCount = node->GetChildCount();
    for (int i = 0; i < childCount; ++i) {
        perse(assets, node->GetChild(i), indent + 1);
    }
}

void FBX::getIndex(FbxMesh* mesh) {
    auto polygonCount = mesh->GetPolygonCount();

    mIndexArray.reserve(polygonCount * 3);

    for (int i = 0; i < polygonCount; i++) {
        mIndexArray.emplace_back(mesh->GetPolygonVertex(i, 0));
        mIndexArray.emplace_back(mesh->GetPolygonVertex(i, 1));
        mIndexArray.emplace_back(mesh->GetPolygonVertex(i, 2));
    }
}

void FBX::getVertex(FbxMesh* mesh) {
    //頂点数
    int vertexNum = mesh->GetControlPointsCount();
    int uvNum = mesh->GetTextureUVCount();
    mVertexArray->setNumVerts(vertexNum);
    mVertexArray->setNumTex(uvNum);

    if (uvNum > vertexNum) {
        mVertices = new MeshVertex[uvNum];
    } else {
        mVertices = new MeshVertex[vertexNum];
    }

    //頂点座標配列
    //FbxVector4* src = mesh->GetControlPoints();
    //for (size_t i = 0; i < vertexNum; i++) {
    //    mVertices[i].pos.x = static_cast<float>(-src[i][0]);
    //    mVertices[i].pos.y = static_cast<float>(src[i][1]);
    //    mVertices[i].pos.z = static_cast<float>(src[i][2]);
    //}

    //for (const auto& index : mIndexArray) {
    //    auto controlPoint = mesh->GetControlPointAt(index);

    //    mVertices[index].pos.x = static_cast<float>(-controlPoint[0]);
    //    mVertices[index].pos.y = static_cast<float>(controlPoint[1]);
    //    mVertices[index].pos.z = static_cast<float>(controlPoint[2]);
    //}

    FbxVector4* vertex = mesh->GetControlPoints();
    bool moreUVs = uvNum > vertexNum;
    for (int i = 0; i < mesh->GetPolygonCount(); i++) {
        int startIndex = mesh->GetPolygonVertexIndex(i);
        int index0 = (moreUVs) ? mesh->GetTextureUVIndex(i, 0) : mIndexArray[startIndex];
        int index1 = (moreUVs) ? mesh->GetTextureUVIndex(i, 1) : mIndexArray[startIndex + 1];
        int index2 = (moreUVs) ? mesh->GetTextureUVIndex(i, 2) : mIndexArray[startIndex + 2];

        int index = mesh->GetPolygonVertex(i, 0);
        mVertices[index0].pos.x = static_cast<float>(-vertex[index][0]);
        mVertices[index0].pos.y = static_cast<float>(vertex[index][1]);
        mVertices[index0].pos.z = static_cast<float>(vertex[index][2]);

        index = mesh->GetPolygonVertex(i, 1);
        mVertices[index1].pos.x = static_cast<float>(-vertex[index][0]);
        mVertices[index1].pos.y = static_cast<float>(vertex[index][1]);
        mVertices[index1].pos.z = static_cast<float>(vertex[index][2]);

        index = mesh->GetPolygonVertex(i, 2);
        mVertices[index2].pos.x = static_cast<float>(-vertex[index][0]);
        mVertices[index2].pos.y = static_cast<float>(vertex[index][1]);
        mVertices[index2].pos.z = static_cast<float>(vertex[index][2]);
    }
}

void FBX::getNormals(FbxMesh* mesh) {
    //FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();
    //if (!normalElement) {
    //    return;
    //}

    //if (normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
    //    for (size_t i = 0; i < mesh->GetControlPointsCount(); i++) {
    //        int normalIndex = 0;
    //        if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
    //            normalIndex = i;
    //        }
    //        if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
    //            normalIndex = normalElement->GetIndexArray().GetAt(i);
    //        }

    //        FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);
    //        mVertices[normalIndex].normal.x = static_cast<float>(-normal[0]);
    //        mVertices[normalIndex].normal.y = static_cast<float>(normal[1]);
    //        mVertices[normalIndex].normal.z = static_cast<float>(normal[2]);
    //    }
    //} else if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
    //    int indexByPolygonVertex = 0;
    //    for (size_t i = 0; i < mesh->GetPolygonCount(); i++) {
    //        int polygonSize = mesh->GetPolygonSize(i);
    //        for (size_t j = 0; j < polygonSize; j++) {
    //            int normalIndex = 0;
    //            if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
    //                normalIndex = indexByPolygonVertex;
    //            }
    //            if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
    //                normalIndex = normalElement->GetIndexArray().GetAt(indexByPolygonVertex);
    //            }

    //            FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);
    //            mVertices[normalIndex].normal.x = static_cast<float>(-normal[0]);
    //            mVertices[normalIndex].normal.y = static_cast<float>(normal[1]);
    //            mVertices[normalIndex].normal.z = static_cast<float>(normal[2]);

    //            indexByPolygonVertex++;
    //        }
    //    }
    //}

    //FbxVector4 normal;
    //for (int i = 0; i < mesh->GetPolygonCount(); i++) {
    //    int startIndex = mesh->GetPolygonVertexIndex(i);

    //    int vertIndex0 = mIndexArray[startIndex];
    //    int vertIndex1 = mIndexArray[startIndex + 1];
    //    int vertIndex2 = mIndexArray[startIndex + 2];

    //    mesh->GetPolygonVertexNormal(i, 0, normal);
    //    mVertices[vertIndex0].normal.x = -normal[0];
    //    mVertices[vertIndex0].normal.y = normal[1];
    //    mVertices[vertIndex0].normal.z = normal[2];

    //    mesh->GetPolygonVertexNormal(i, 1, normal);
    //    mVertices[vertIndex1].normal.x = -normal[0];
    //    mVertices[vertIndex1].normal.y = normal[1];
    //    mVertices[vertIndex1].normal.z = normal[2];

    //    mesh->GetPolygonVertexNormal(i, 2, normal);
    //    mVertices[vertIndex2].normal.x = -normal[0];
    //    mVertices[vertIndex2].normal.y = normal[1];
    //    mVertices[vertIndex2].normal.z = normal[2];
    //}

    FbxVector4 normal;
    bool moreUVs = mVertexArray->getNumTex() > mVertexArray->getNumVerts();
    for (int i = 0; i < mesh->GetPolygonCount(); i++) {
        int startIndex = mesh->GetPolygonVertexIndex(i);
        int index0 = (moreUVs) ? mesh->GetTextureUVIndex(i, 0) : mIndexArray[startIndex];
        int index1 = (moreUVs) ? mesh->GetTextureUVIndex(i, 1) : mIndexArray[startIndex + 1];
        int index2 = (moreUVs) ? mesh->GetTextureUVIndex(i, 2) : mIndexArray[startIndex + 2];

        mesh->GetPolygonVertexNormal(i, 0, normal);
        mVertices[index0].normal.x = static_cast<float>(-normal[0]);
        mVertices[index0].normal.y = static_cast<float>(normal[1]);
        mVertices[index0].normal.z = static_cast<float>(normal[2]);

        mesh->GetPolygonVertexNormal(i, 1, normal);
        mVertices[index1].normal.x = static_cast<float>(-normal[0]);
        mVertices[index1].normal.y = static_cast<float>(normal[1]);
        mVertices[index1].normal.z = static_cast<float>(normal[2]);

        mesh->GetPolygonVertexNormal(i, 2, normal);
        mVertices[index2].normal.x = static_cast<float>(-normal[0]);
        mVertices[index2].normal.y = static_cast<float>(normal[1]);
        mVertices[index2].normal.z = static_cast<float>(normal[2]);
    }
}

void FBX::getUV(FbxMesh* mesh) {
    //FbxStringList uvSetNameList;
    //mesh->GetUVSetNames(uvSetNameList);

    ////すべてのUVセットを反復処理する
    //for (int uvSetIndex = 0; uvSetIndex < uvSetNameList.GetCount(); uvSetIndex++) {
    //    //uvSetIndex-番目のUVセットを取得
    //    const char* uvSetName = uvSetNameList.GetStringAt(uvSetIndex);
    //    const FbxGeometryElementUV* uvElement = mesh->GetElementUV(uvSetName);
    //    if (!uvElement) {
    //        continue;
    //    }

    //    //マッピングモードeByPolygonVertexおよびeByControlPointのみをサポート
    //    if (uvElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex && uvElement->GetMappingMode() != FbxGeometryElement::eByControlPoint) {
    //        return;
    //    }

    //    //インデックス配列。uvデータを参照するインデックスを保持します
    //    const bool useIndex = uvElement->GetReferenceMode() != FbxGeometryElement::eDirect;
    //    const int indexCount = (useIndex) ? uvElement->GetIndexArray().GetCount() : 0;

    //    //ポリゴンごとにデータを反復処理する
    //    const int polyCount = mesh->GetPolygonCount();

    //    if (uvElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
    //        for (int polyIndex = 0; polyIndex < polyCount; polyIndex++) {
    //            //MakePolyに渡す必要がある最大インデックス配列を作成します
    //            const int polySize = mesh->GetPolygonSize(polyIndex);
    //            for (int vertIndex = 0; vertIndex < polySize; vertIndex++) {
    //                //頂点配列の現在の頂点のインデックスを取得します
    //                int polyVertIndex = mesh->GetPolygonVertex(polyIndex, vertIndex);

    //                //UVインデックスは参照モードに依存します
    //                int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyVertIndex) : polyVertIndex;

    //                FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
    //                mVertices[uvIndex].uv.x = static_cast<float>(uv[0]);
    //                mVertices[uvIndex].uv.y = 1.f - static_cast<float>(uv[1]);
    //            }
    //        }
    //    } else if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
    //        int polyIndexCounter = 0;
    //        for (int polyIndex = 0; polyIndex < polyCount; polyIndex++) {
    //            //MakePolyに渡す必要がある最大インデックス配列を作成します
    //            const int polySize = mesh->GetPolygonSize(polyIndex);
    //            for (int vertIndex = 0; vertIndex < polySize; vertIndex++) {
    //                if (polyIndexCounter < indexCount) {
    //                    //UVインデックスは参照モードに依存します
    //                    int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyIndexCounter) : polyIndexCounter;

    //                    FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
    //                    mVertices[uvIndex].uv.x = static_cast<float>(uv[0]);
    //                    mVertices[uvIndex].uv.y = 1.f - static_cast<float>(uv[1]);

    //                    polyIndexCounter++;
    //                }
    //            }
    //        }
    //    }
    //}

    //FbxLayerElementUV* uvElement = mesh->GetLayer(0)->GetUVs();
    //for (int i = 0; i < mesh->GetTextureUVCount(); i++) {
    //    FbxVector2 uv = uvElement->GetDirectArray().GetAt(i);
    //    mVertices[i].uv.x = uv[0];
    //    mVertices[i].uv.y = 1.f - uv[1];
    //}

    FbxLayerElementUV* uvElement = mesh->GetLayer(0)->GetUVs();
    bool moreUVs = mVertexArray->getNumTex() > mVertexArray->getNumVerts();
    if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
        for (int i = 0; i < mesh->GetPolygonCount(); i++) {
            int startIndex = mesh->GetPolygonVertexIndex(i);
            int index0 = (moreUVs) ? mesh->GetTextureUVIndex(i, 0) : mIndexArray[startIndex];
            int index1 = (moreUVs) ? mesh->GetTextureUVIndex(i, 1) : mIndexArray[startIndex + 1];
            int index2 = (moreUVs) ? mesh->GetTextureUVIndex(i, 2) : mIndexArray[startIndex + 2];

            int uvIndex = mesh->GetTextureUVIndex(i, 0);
            FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
            mVertices[index0].uv.x = static_cast<float>(uv[0]);
            mVertices[index0].uv.y = 1.f - static_cast<float>(uv[1]);

            uvIndex = mesh->GetTextureUVIndex(i, 1);
            uv = uvElement->GetDirectArray().GetAt(uvIndex);
            mVertices[index1].uv.x = static_cast<float>(uv[0]);
            mVertices[index1].uv.y = 1.f - static_cast<float>(uv[1]);

            uvIndex = mesh->GetTextureUVIndex(i, 2);
            uv = uvElement->GetDirectArray().GetAt(uvIndex);
            mVertices[index2].uv.x = static_cast<float>(uv[0]);
            mVertices[index2].uv.y = 1.f - static_cast<float>(uv[1]);
        }
    } else if (uvElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
        for (int i = 0; i < mVertexArray->getNumTex(); i++) {
            FbxVector2 uv = uvElement->GetDirectArray().GetAt(i);
            mVertices[i].uv.x = static_cast<float>(uv[0]);
            mVertices[i].uv.y = 1.f - static_cast<float>(uv[1]);
        }
    }
}

void FBX::getMaterial(std::shared_ptr<AssetsManager> assets, FbxMesh* mesh) {
    FbxNode* node = mesh->GetNode();
    if (!node) {
        return;
    }

    //マテリアルの数
    int materialNum = node->GetMaterialCount();
    if (materialNum == 0) {
        return;
    }

    //マテリアル情報を取得
    for (size_t i = 0; i < materialNum; i++) {
        mMaterials.emplace_back(std::make_shared<Material>());

        FbxSurfaceMaterial* material = node->GetMaterial(i);
        if (!material) {
            continue;
        }

        //解析
        //LambertかPhongか
        if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
            FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

            auto mat = mMaterials[i];

            //アンビエント
            auto ambient = lambert->Ambient.Get();
            mat->ambient.x = static_cast<float>(ambient[0]);
            mat->ambient.y = static_cast<float>(ambient[1]);
            mat->ambient.z = static_cast<float>(ambient[2]);
            mat->ambient.w = 1.f;

            //ディヒューズ
            auto diffuse = lambert->Diffuse.Get();
            mat->diffuse.x = static_cast<float>(diffuse[0]);
            mat->diffuse.y = static_cast<float>(diffuse[1]);
            mat->diffuse.z = static_cast<float>(diffuse[2]);
            mat->diffuse.w = 1.f;

            //エミッシブ
            auto emissive = lambert->Emissive.Get();
            mat->emissive.x = static_cast<float>(emissive[0]);
            mat->emissive.y = static_cast<float>(emissive[1]);
            mat->emissive.z = static_cast<float>(emissive[2]);
            mat->emissive.w = 1.f;

            //バンプ
            auto bump = lambert->Bump.Get();
            mat->bump.x = static_cast<float>(bump[0]);
            mat->bump.y = static_cast<float>(bump[1]);
            mat->bump.z = static_cast<float>(bump[2]);
            mat->bump.w = 1.f;

            //透過度
            mat->transparency = static_cast<float>(lambert->TransparencyFactor.Get());
        } else if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
            FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(material);

            auto mat = mMaterials[i];

            //アンビエント
            auto ambient = phong->Ambient.Get();
            mat->ambient.x = static_cast<float>(ambient[0]);
            mat->ambient.y = static_cast<float>(ambient[1]);
            mat->ambient.z = static_cast<float>(ambient[2]);
            mat->ambient.w = 1.f;

            //ディヒューズ
            auto diffuse = phong->Diffuse.Get();
            mat->diffuse.x = static_cast<float>(diffuse[0]);
            mat->diffuse.y = static_cast<float>(diffuse[1]);
            mat->diffuse.z = static_cast<float>(diffuse[2]);
            mat->diffuse.w = 1.f;

            //エミッシブ
            auto emissive = phong->Emissive.Get();
            mat->emissive.x = static_cast<float>(emissive[0]);
            mat->emissive.y = static_cast<float>(emissive[1]);
            mat->emissive.z = static_cast<float>(emissive[2]);
            mat->emissive.w = 1.f;

            //バンプ
            auto bump = phong->Bump.Get();
            mat->bump.x = static_cast<float>(bump[0]);
            mat->bump.y = static_cast<float>(bump[1]);
            mat->bump.z = static_cast<float>(bump[2]);
            mat->bump.w = 1.f;

            //透過度
            mat->transparency = static_cast<float>(phong->TransparencyFactor.Get());

            //スペキュラ
            auto specular = phong->Specular.Get();
            mat->specular.x = static_cast<float>(specular[0]);
            mat->specular.y = static_cast<float>(specular[1]);
            mat->specular.z = static_cast<float>(specular[2]);
            mat->specular.w = 1.f;

            //光沢
            mat->shininess = static_cast<float>(phong->Shininess.Get());
        }

        //ディフューズプロパティを検索
        FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

        //プロパティが持っているレイヤードテクスチャの枚数をチェック
        int layerNum = property.GetSrcObjectCount<FbxLayeredTexture>();

        //レイヤードテクスチャが無ければ通常テクスチャ
        if (layerNum == 0) {
            //通常テクスチャの枚数をチェック
            int numGeneralTexture = property.GetSrcObjectCount<FbxTexture>();

            //各テクスチャについてテクスチャ情報をゲット
            for (int j = 0; j < numGeneralTexture; ++j) {
                //j番目のテクスチャオブジェクト取得
                FbxTexture* texture = FbxCast<FbxTexture>(property.GetSrcObject<FbxTexture>(j));

                //テクスチャ名
                mMaterials[i]->textureName = texture->GetName();
                //テクスチャーを作成
                mMaterials[i]->texture = assets->createTexture(mMaterials[i]->textureName, false);

                break; //とりあえず今は1枚だけサポート
            }
        }

        //マテリアルの数だけインデックスバッファーを作成
        int count = 0;
        int* indices = new int[mesh->GetPolygonVertexCount()]; //メッシュ内のポリゴン数でメモリ確保

        //そのマテリアルであるインデックス配列内の開始インデックスを調べる + インデックスの個数も調べる
        bool moreUVs = mVertexArray->getNumTex() > mVertexArray->getNumVerts();
        for (int j = 0; j < mesh->GetPolygonCount(); j++) {
            FbxLayerElementMaterial* mat = mesh->GetLayer(0)->GetMaterials(); //レイヤーが1枚だけを想定
            int matId = mat->GetIndexArray().GetAt(j);
            if (matId == i) {
                indices[count] = (moreUVs) ? mesh->GetTextureUVIndex(j, 0) : mesh->GetPolygonVertex(j, 0);
                indices[count + 1] = (moreUVs) ? mesh->GetTextureUVIndex(j, 1) : mesh->GetPolygonVertex(j, 1);
                indices[count + 2] = (moreUVs) ? mesh->GetTextureUVIndex(j, 2) : mesh->GetPolygonVertex(j, 2);
                count += 3;
            }
        }
        mVertexArray->createIndexBuffer(i, count, indices);
        mMaterials[i]->numFace = count / 3; //そのマテリアル内のポリゴン数

        delete[] indices;
    }
}
