﻿#include "ChickenMeshComponent.h"
#include "Camera.h"
#include "../Device/AssetsManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Mesh/IMeshLoader.h"
#include "../Mesh/Material.h"
#include "../Mesh/VertexArray.h"
#include "../Shader/Shader.h"
#include "../System/DirectX.h"
#include "../System/Texture.h"

ChickenMeshComponent::ChickenMeshComponent(std::shared_ptr<GameObject> owner) :
    MeshComponent(owner, "ChickenMeshComponent"),
    mUpColor(ColorPalette::white),
    mBottomColor(ColorPalette::white),
    mLeftColor(ColorPalette::white),
    mRightColor(ColorPalette::white),
    mForeColor(ColorPalette::white),
    mBackColor(ColorPalette::white) {
}

ChickenMeshComponent::~ChickenMeshComponent() = default;

void ChickenMeshComponent::setMesh(const std::string & fileName) {
    mMesh = Singleton<AssetsManager>::instance().createMesh(fileName);
    addToManager(true);
}

void ChickenMeshComponent::setShader() {
    mShader = Singleton<AssetsManager>::instance().createShader("ChickenMesh.hlsl");
    //メッシュ用コンスタントバッファの作成
    mShader->createConstantBuffer(sizeof(ChickenMeshConstantBuffer), 0);
    mShader->createConstantBuffer(sizeof(MaterialConstantBuffer), 1);
    //インプットレイアウトの生成
    constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        //{ "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 6, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    mShader->createInputLayout(layout, numElements);
}

void ChickenMeshComponent::draw() {
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
        ChickenMeshConstantBuffer cb;
        //ワールド行列を渡す
        auto trans = owner()->transform();
        cb.world = trans->getWorldTransform();
        cb.world.transpose();
        cb.pos = trans->getPosition();
        //ワールド、カメラ、射影行列を渡す
        cb.wvp = trans->getWorldTransform() * mCamera->getViewProjection();
        cb.wvp.transpose();
        //色
        cb.upColor = mUpColor;
        cb.bottomColor = mBottomColor;
        cb.leftColor = mLeftColor;
        cb.rightColor = mRightColor;
        cb.foreColor = mForeColor;
        cb.backColor = mBackColor;

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
    for (size_t i = 0; i < mMesh->getNumMaterial(); i++) {
        //使用されていないマテリアル対策
        auto mat = mMesh->getMaterial(i);
        if (mat->numFace == 0) {
            continue;
        }
        //インデックスバッファーをセット
        vertArray->setIndexBuffer(i);

        if (mShader->map(&msrd, 1)) {
            MaterialConstantBuffer cb;
            cb.diffuse = Vector4(mColor, 1.f);
            cb.specular = mat->specular;

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

void ChickenMeshComponent::setUpColor(const Vector3 & color) {
    mUpColor = color;
}

void ChickenMeshComponent::setBottomColor(const Vector3 & color) {
    mBottomColor = color;
}

void ChickenMeshComponent::setLeftColor(const Vector3 & color) {
    mLeftColor = color;
}

void ChickenMeshComponent::setRightColor(const Vector3 & color) {
    mRightColor = color;
}

void ChickenMeshComponent::setForeColor(const Vector3 & color) {
    mForeColor = color;
}

void ChickenMeshComponent::setBackColor(const Vector3 & color) {
    mBackColor = color;
}
