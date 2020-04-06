#include "MeshComponent.h"
#include "Camera.h"
#include "ComponentManager.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/AssetsManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Mesh/Material.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/MeshManager.h"
#include "../Mesh/VertexArray.h"
#include "../Shader/Shader.h"
#include "../System/DirectX.h"
#include "../System/Texture.h"
#include "../Utility/LevelLoader.h"

MeshComponent::MeshComponent(std::shared_ptr<GameObject> owner, const std::string& type) :
    Component(owner, type),
    mMesh(nullptr),
    mShader(nullptr),
    mCamera(nullptr),
    mColor(ColorPalette::white) {
}

MeshComponent::~MeshComponent() {
    mMesh->destroy();
}

void MeshComponent::start() {
    auto camera = owner()->getGameObjectManager()->find("Camera");
    mCamera = camera->componentManager()->getComponent<Camera>();

    setShader();
}

void MeshComponent::onSetActive(bool value) {
    setActive(value);
}

void MeshComponent::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    std::string fileName;
    if (JsonHelper::getString(inObj, "fileName", &fileName)) {
        setMesh(fileName);
    }
}

void MeshComponent::drawDebugInfo(debugInfoList* inspect) const {
    debugInfo info;
    info.first = "Color";
    info.second = InspectHelper::vector3ToString(mColor);
    inspect->emplace_back(info);
}

void MeshComponent::setMesh(const std::string& fileName) {
    mMesh = std::make_shared<Mesh>(fileName);
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

    auto meshData = mMesh->getMeshData();

    //バーテックスバッファーをセット
    meshData->getVertexArray()->setVertexBuffer();

    //このコンスタントバッファーを使うシェーダーの登録
    mShader->setPSConstantBuffers(1);

    //マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    for (size_t i = 0; i < meshData->getNumMaterial(); i++) {
        //使用されていないマテリアル対策
        auto mat = meshData->getMaterial(i);
        if (mat->numFace == 0) {
            continue;
        }
        //インデックスバッファーをセット
        meshData->getVertexArray()->setIndexBuffer(i);

        if (mShader->map(&msrd, 1)) {
            MaterialConstantBuffer cb;
            cb.diffuse = mat->diffuse;
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

const Vector3& MeshComponent::getCenter() const {
    return mMesh->getCenter();
}

float MeshComponent::getRadius() const {
    return mMesh->getRadius();
}

void MeshComponent::setActive(bool value) {
    mMesh->setActive(value);
}

bool MeshComponent::getActive() const {
    return mMesh->getActive();
}

bool MeshComponent::isDead() {
    return mMesh->isDead();
}

void MeshComponent::setColor(const Vector3& color) {
    mColor = color;
}

const Vector3& MeshComponent::getColor() const {
    return mColor;
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
