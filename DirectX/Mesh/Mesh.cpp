﻿#include "Mesh.h"
#include "Material.h"
#include "MeshLoader.h"
#include "MeshManager.h"
#include "../Actor/Transform3D.h"
#include "../Camera/Camera.h"
#include "../Device/AssetsManager.h"
#include "../Device/Renderer.h"
#include "../Shader/Shader.h"
#include "../System/Buffer.h"
#include "../System/DirectX.h"
#include "../System/GBuffer.h"
#include "../System/InputElement.h"
#include "../System/SubResourceDesc.h"
#include "../System/Texture.h"
#include "../System/VertexArray.h"

Mesh::Mesh(std::shared_ptr<Renderer> renderer, const std::string& fileName) :
    mLoader(renderer->getAssetsManager()->createMesh(renderer, fileName)),
    mTransform(nullptr),
    mShader(renderer->getAssetsManager()->createShader("GBuffer.hlsl")),
    mState(State::ACTIVE) {

    //メッシュ用コンスタントバッファの作成
    mShader->createConstantBuffer(sizeof(MeshConstantBuffer), 0);
    mShader->createConstantBuffer(sizeof(MaterialConstantBuffer), 1);

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

void Mesh::createSphere(std::shared_ptr<Sphere> * sphere) const {
    mLoader->createSphere(sphere);
}

void Mesh::draw(std::shared_ptr<Camera> camera) const {
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
        MeshConstantBuffer cb;
        //ワールド行列を渡す
        cb.world = mTransform->getWorldTransform();
        cb.world.transpose();
        //ワールド、カメラ、射影行列を渡す
        cb.WVP = mTransform->getWorldTransform() * camera->getView() * camera->getProjection();
        cb.WVP.transpose();

        memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
        mShader->unmap(0);
    }

    //バーテックスバッファーをセット
    mLoader->setVertexBuffer();

    //このコンスタントバッファーを使うシェーダーの登録
    mShader->setPSConstantBuffers(1);

    //マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    for (size_t i = 0; i < mLoader->getMaterialSize(); i++) {
        //使用されていないマテリアル対策
        if (mLoader->getMaterialData(i)->numFace == 0) {
            continue;
        }
        //インデックスバッファーをセット
        mLoader->setIndexBuffer(i);

        if (mShader->map(&msrd, 1)) {
            MaterialConstantBuffer cb;
            cb.diffuse = mLoader->getMaterialData(i)->Kd;
            cb.specular = mLoader->getMaterialData(i)->Ks;

            if (auto t = mLoader->getMaterialData(i)->texture) {
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
        Singleton<DirectX>::instance().drawIndexed(mLoader->getMaterialData(i)->numFace * 3);
    }
}

void Mesh::setTransform(std::shared_ptr<Transform3D> transform) {
    mTransform = transform;
}

std::shared_ptr<MeshLoader> Mesh::getMeshData() const {
    return mLoader;
}

void Mesh::destroy() {
    mState = State::DEAD;
}

void Mesh::setActive(bool value) {
    mState = (value) ? State::ACTIVE : State::NON_ACTIVE;
}

bool Mesh::getActive() const {
    return mState == State::ACTIVE;
}

bool Mesh::isDead() const {
    return mState == State::DEAD;
}

void Mesh::setMeshManager(MeshManager * manager) {
    mMeshManager = manager;
}

MeshManager* Mesh::mMeshManager = nullptr;
