#include "MeshComponent.h"
#include "Camera.h"
#include "ComponentManager.h"
#include "../Device/AssetsManager.h"
#include "../DirectX/DirectX.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Mesh/IMeshLoader.h"
#include "../Mesh/Material.h"
#include "../Mesh/MeshManager.h"
#include "../Mesh/VertexArray.h"
#include "../System/Shader.h"
#include "../System/Texture.h"
#include "../Utility/LevelLoader.h"

MeshComponent::MeshComponent() :
    Component(),
    mMesh(nullptr),
    mShader(nullptr),
    mCamera(nullptr),
    mState(State::ACTIVE),
    mCenter(Vector3::zero),
    mRadius(0.f),
    mColor(ColorPalette::white) {
}

MeshComponent::~MeshComponent() = default;

void MeshComponent::start() {
    const auto& camera = owner()->getGameObjectManager()->find("Camera");
    mCamera = camera->componentManager()->getComponent<Camera>();

    //半径と中心座標の取得
    mCenter = mMesh->getCenter();
    mRadius = mMesh->getRadius();

    setShader();
}

void MeshComponent::onSetActive(bool value) {
    setActive(value);
}

void MeshComponent::loadProperties(const rapidjson::Value& inObj) {
    std::string fileName;
    if (JsonHelper::getString(inObj, "fileName", &fileName)) {
        setMesh(fileName);
    }
}

void MeshComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("Color", mColor);
}

bool MeshComponent::isVisible() const {
    if (!mCamera) {
        return false;
    }

    return mCamera->viewFrustumCulling(owner()->transform()->getPosition(), mRadius);
}

size_t MeshComponent::getNumMaterial() const {
    return mMaterials.size();
}

std::shared_ptr<Material> MeshComponent::getMaterial(unsigned index) const {
    return mMaterials[index];
}

const Vector3& MeshComponent::getCenter() const {
    return mCenter;
}

float MeshComponent::getRadius() const {
    return mRadius;
}

void MeshComponent::setColor(const Vector3& color) {
    mColor = color;
}

const Vector3& MeshComponent::getColor() const {
    return mColor;
}

void MeshComponent::destroy() {
    mState = State::DEAD;
}

void MeshComponent::setActive(bool value) {
    mState = (value) ? State::ACTIVE : State::NON_ACTIVE;
}

bool MeshComponent::getActive() const {
    return mState == State::ACTIVE;
}

bool MeshComponent::isDead() const {
    return mState == State::DEAD;
}

void MeshComponent::setMesh(const std::string& fileName) {
    mMesh = Singleton<AssetsManager>::instance().createMesh(fileName);
    mMesh->setInitMaterials(&mMaterials);
    addToManager();
}

void MeshComponent::setShader() {
    mShader = Singleton<AssetsManager>::instance().createShader("GBuffer.hlsl");
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
}

void MeshComponent::draw() {
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
        auto trans = owner()->transform();
        cb.world = trans->getWorldTransform();
        cb.world.transpose();
        //ワールド、カメラ、射影行列を渡す
        cb.WVP = trans->getWorldTransform() * mCamera->getViewProjection();
        cb.WVP.transpose();

        memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
        mShader->unmap(0);
    }

    //バーテックスバッファーをセット
    mMesh->getVertexArray()->setVertexBuffer();

    //このコンスタントバッファーを使うシェーダーの登録
    mShader->setPSConstantBuffers(1);

    //マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    for (size_t i = 0; i < getNumMaterial(); i++) {
        //使用されていないマテリアル対策
        auto mat = getMaterial(i);
        if (mat->numFace == 0) {
            continue;
        }
        //インデックスバッファーをセット
        mMesh->getVertexArray()->setIndexBuffer(i);

        if (mShader->map(&msrd, 1)) {
            MaterialConstantBuffer cb;
            cb.diffuse = Vector4(mat->diffuse, 1.f);
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

void MeshComponent::setMeshManager(MeshManager* manager) {
    mMeshManager = manager;
}

void MeshComponent::addToManager(bool isTransparent) {
    if (!mMeshManager) {
        return;
    }

    if (isTransparent) {
        mMeshManager->addTransparent(shared_from_this());
    } else {
        mMeshManager->add(shared_from_this());
    }
}

MeshManager* MeshComponent::mMeshManager = nullptr;
