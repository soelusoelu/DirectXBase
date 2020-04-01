#include "Mesh.h"
#include "Material.h"
#include "MeshManager.h"
#include "VertexArray.h"
#include "../Component/Camera.h"
#include "../Device/AssetsManager.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Light/DirectionalLight.h"
#include "../Shader/Shader.h"
#include "../System/Buffer.h"
#include "../System/DirectX.h"
#include "../System/GBuffer.h"
#include "../System/InputElement.h"
#include "../System/SubResourceDesc.h"
#include "../System/Texture.h"

Mesh::Mesh(std::shared_ptr<Renderer> renderer, const std::string& fileName) :
    mMesh(renderer->getAssetsManager()->createMesh(fileName)),
    mTransform(nullptr),
    mShader(renderer->getAssetsManager()->createShader("GBuffer.hlsl")),
    mShaderTransparent(renderer->getAssetsManager()->createShader("Mesh.hlsl")),
    mState(State::ACTIVE),
    mRadius(0.f),
    mCenter(Vector3::zero),
    mAlpha(1.f) {

    //メッシュ用コンスタントバッファの作成
    mShader->createConstantBuffer(sizeof(MeshConstantBuffer), 0);
    mShader->createConstantBuffer(sizeof(MaterialConstantBuffer), 1);
    mShaderTransparent->createConstantBuffer(sizeof(TransparentConstantBuffer), 0);
    mShaderTransparent->createConstantBuffer(sizeof(MaterialConstantBuffer), 1);

    //インプットレイアウトの生成
    constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 6, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    mShader->createInputLayout(layout, numElements);
    mShaderTransparent->createInputLayout(layout, numElements);

    //半径と中心座標の取得
    mCenter = mMesh->getCenter();
    mRadius = mMesh->getRadius();
}

Mesh::~Mesh() = default;

void Mesh::addToManager(bool isTransparent) {
    if (!mMeshManager) {
        return;
    }

    if (isTransparent) {
        mMeshManager->addTransparent(shared_from_this());
    } else {
        mMeshManager->add(shared_from_this());
    }
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
        cb.WVP = mTransform->getWorldTransform() * camera->getViewProjection();
        cb.WVP.transpose();

        memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
        mShader->unmap(0);
    }

    //バーテックスバッファーをセット
    mMesh->getVertexArray()->setVertexBuffer();

    //このコンスタントバッファーを使うシェーダーの登録
    mShader->setPSConstantBuffers(1);

    //マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    for (size_t i = 0; i < mMesh->getNumMaterial(); i++) {
        //使用されていないマテリアル対策
        if (mMesh->getMaterial(i)->numFace == 0) {
            continue;
        }
        //インデックスバッファーをセット
        mMesh->getVertexArray()->setIndexBuffer(i);

        if (mShader->map(&msrd, 1)) {
            MaterialConstantBuffer cb;
            cb.diffuse = mMesh->getMaterial(i)->diffuse;
            cb.specular = mMesh->getMaterial(i)->specular;

            if (auto t = mMesh->getMaterial(i)->texture) {
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
        Singleton<DirectX>::instance().drawIndexed(mMesh->getMaterial(i)->numFace * 3);
    }
}

void Mesh::drawTransparent(std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera) const {
    //使用するシェーダーの登録
    mShaderTransparent->setVSShader();
    mShaderTransparent->setPSShader();
    //このコンスタントバッファーを使うシェーダーの登録
    mShaderTransparent->setVSConstantBuffers(0);
    mShaderTransparent->setPSConstantBuffers(0);
    //頂点インプットレイアウトをセット
    mShaderTransparent->setInputLayout();

    //シェーダーのコンスタントバッファーに各種データを渡す
    MappedSubResourceDesc msrd;
    if (mShaderTransparent->map(&msrd, 0)) {
        TransparentConstantBuffer cb;
        //ワールド行列を渡す
        cb.world = mTransform->getWorldTransform();
        cb.world.transpose();
        //ワールド、カメラ、射影行列を渡す
        cb.WVP = mTransform->getWorldTransform() * camera->getViewProjection();
        cb.WVP.transpose();
        cb.lightDir = renderer->getDirectionalLight()->getDirection();
        cb.cameraPos = camera->owner()->transform()->getPosition();

        memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
        mShaderTransparent->unmap(0);
    }

    //バーテックスバッファーをセット
    mMesh->getVertexArray()->setVertexBuffer();

    //このコンスタントバッファーを使うシェーダーの登録
    mShaderTransparent->setVSConstantBuffers(1);
    mShaderTransparent->setPSConstantBuffers(1);

    //マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    for (size_t i = 0; i < mMesh->getNumMaterial(); i++) {
        //使用されていないマテリアル対策
        if (mMesh->getMaterial(i)->numFace == 0) {
            continue;
        }
        //インデックスバッファーをセット
        mMesh->getVertexArray()->setIndexBuffer(i);

        if (mShaderTransparent->map(&msrd, 1)) {
            MaterialConstantBuffer cb;
            cb.diffuse = mMesh->getMaterial(i)->diffuse;
            cb.diffuse.w = mAlpha;
            cb.specular = mMesh->getMaterial(i)->specular;

            if (auto t = mMesh->getMaterial(i)->texture) {
                t->setPSTextures();
                t->setPSSamplers();
                cb.textureFlag = 1;
            } else {
                cb.textureFlag = 0;
            }

            memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
            mShaderTransparent->unmap(1);
        }

        //プリミティブをレンダリング
        Singleton<DirectX>::instance().drawIndexed(mMesh->getMaterial(i)->numFace * 3);
    }
}

void Mesh::setTransform(std::shared_ptr<Transform3D> transform) {
    mTransform = transform;
}

std::shared_ptr<IMeshLoader> Mesh::getMeshData() const {
    return mMesh;
}

float Mesh::getRadius() const {
    return mRadius;
}

const Vector3& Mesh::getCenter() const {
    return mCenter;
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

void Mesh::setAlpha(float alpha) {
    mAlpha = alpha;
}

float Mesh::getAlpha() const {
    return mAlpha;
}

void Mesh::setMeshManager(MeshManager * manager) {
    mMeshManager = manager;
}

MeshManager* Mesh::mMeshManager = nullptr;
