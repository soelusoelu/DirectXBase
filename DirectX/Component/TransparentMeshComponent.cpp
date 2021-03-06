﻿#include "TransparentMeshComponent.h"
#include "Camera.h"
#include "ComponentManager.h"
#include "DirectionalLight.h"
#include "../Device/AssetsManager.h"
#include "../DirectX/DirectX.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Mesh/IMeshLoader.h"
#include "../Mesh/Material.h"
#include "../Mesh/VertexArray.h"
#include "../System/Shader.h"
#include "../System/Texture.h"
#include "../Utility/LevelLoader.h"

TransparentMeshComponent::TransparentMeshComponent() :
    MeshComponent(),
    mDirLight(nullptr),
    mAlpha(1.f) {
}

TransparentMeshComponent::~TransparentMeshComponent() = default;

void TransparentMeshComponent::start() {
    MeshComponent::start();

    const auto& dirLight = owner()->getGameObjectManager()->find("DirectionalLight");
    mDirLight = dirLight->componentManager()->getComponent<DirectionalLight>();
}

void TransparentMeshComponent::loadProperties(const rapidjson::Value & inObj) {
    MeshComponent::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "alpha", &mAlpha);
}

void TransparentMeshComponent::drawDebugInfo(ComponentDebug::DebugInfoList * inspect) const {
    MeshComponent::drawDebugInfo(inspect);

    inspect->emplace_back("Alpha", mAlpha);
}

void TransparentMeshComponent::setMesh(const std::string & fileName) {
    mMesh = Singleton<AssetsManager>::instance().createMesh(fileName);
    mMesh->setInitMaterials(&mMaterials);
    addToManager(true);
}

void TransparentMeshComponent::setShader() {
    mShader = Singleton<AssetsManager>::instance().createShader("Mesh.hlsl");
    //メッシュ用コンスタントバッファの作成
    mShader->createConstantBuffer(sizeof(TransparentConstantBuffer), 0);
    mShader->createConstantBuffer(sizeof(MaterialConstantBuffer), 1);
    //インプットレイアウトの生成
    constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 6, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    mShader->createInputLayout(layout, numElements);
}

void TransparentMeshComponent::draw() {
    //使用するシェーダーの登録
    mShader->setVSShader();
    mShader->setPSShader();
    //このコンスタントバッファーを使うシェーダーの登録
    mShader->setVSConstantBuffers(0);
    mShader->setPSConstantBuffers(0);
    //頂点インプットレイアウトをセット
    mShader->setInputLayout();

    //シェーダーのコンスタントバッファーに各種データを渡す
    MappedSubResourceDesc msrd;
    if (mShader->map(&msrd, 0)) {
        TransparentConstantBuffer cb;
        //ワールド行列を渡す
        auto trans = owner()->transform();
        cb.world = trans->getWorldTransform();
        cb.world.transpose();
        //ワールド、カメラ、射影行列を渡す
        cb.WVP = trans->getWorldTransform() * mCamera->getViewProjection();
        cb.WVP.transpose();
        cb.lightDir = mDirLight->getDirection();
        cb.cameraPos = mCamera->owner()->transform()->getPosition();

        memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
        mShader->unmap(0);
    }

    auto vertArray = mMesh->getVertexArray();

    //バーテックスバッファーをセット
    vertArray->setVertexBuffer();

    //このコンスタントバッファーを使うシェーダーの登録
    mShader->setVSConstantBuffers(1);
    mShader->setPSConstantBuffers(1);

    //マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    for (size_t i = 0; i < getNumMaterial(); i++) {
        //使用されていないマテリアル対策
        auto mat = getMaterial(i);
        if (mat->numFace == 0) {
            continue;
        }
        //インデックスバッファーをセット
        vertArray->setIndexBuffer(i);

        if (mShader->map(&msrd, 1)) {
            MaterialConstantBuffer cb;
            cb.diffuse = Vector4(mat->diffuse, mAlpha);
            //cb.diffuse = Vector4(mColor, mAlpha);
            cb.specular = Vector4(mat->specular, 1.f);

            if (auto t = mat->texture) {
                t->setPSTextures();
                t->setPSSamplers();
                cb.textureFlag = 1;
            } else {
                cb.textureFlag = 0;
            }

            memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
            mShader->unmap(1);
        }

        //プリミティブをレンダリング
        Singleton<DirectX>::instance().drawIndexed(mat->numFace * 3);
    }
}

void TransparentMeshComponent::setAlpha(float alpha) {
    mAlpha = alpha;
}

float TransparentMeshComponent::getAlpha() const {
    return mAlpha;
}
