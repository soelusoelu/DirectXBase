#include "MeshComponent.h"
#include "Camera.h"
#include "ComponentManager.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/AssetsManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Mesh/IMeshLoader.h"
#include "../Mesh/Material.h"
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
    mState(State::ACTIVE),
    mCenter(Vector3::zero),
    mRadius(0.f),
    mColor(ColorPalette::white) {
}

MeshComponent::~MeshComponent() = default;

void MeshComponent::start() {
    auto camera = owner()->getGameObjectManager()->find("Camera");
    mCamera = camera->componentManager()->getComponent<Camera>();

    //���a�ƒ��S���W�̎擾
    mCenter = mMesh->getCenter();
    mRadius = mMesh->getRadius();

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

void MeshComponent::drawDebugInfo(DebugInfoList* inspect) const {
    DebugInfo info;
    info.first = "Color";
    info.second = InspectHelper::vector3ToString(mColor);
    inspect->emplace_back(info);
}

void MeshComponent::setMesh(const std::string& fileName) {
    mMesh = Singleton<AssetsManager>::instance().createMesh(fileName);
    addToManager();
}

void MeshComponent::setShader() {
    mShader = Singleton<AssetsManager>::instance().createShader("GBuffer.hlsl");
    //���b�V���p�R���X�^���g�o�b�t�@�̍쐬
    mShader->createConstantBuffer(sizeof(MeshConstantBuffer), 0);
    mShader->createConstantBuffer(sizeof(MaterialConstantBuffer), 1);
    //�C���v�b�g���C�A�E�g�̐���
    constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 6, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    mShader->createInputLayout(layout, numElements);
}

void MeshComponent::draw() {
    //�g�p����V�F�[�_�[�̓o�^
    mShader->setVSShader();
    mShader->setPSShader();
    //���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
    mShader->setVSConstantBuffers(0);
    mShader->setPSConstantBuffers(0);
    //���_�C���v�b�g���C�A�E�g���Z�b�g
    mShader->setInputLayout();

    //�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
    MappedSubResourceDesc msrd;
    if (mShader->map(&msrd, 0)) {
        MeshConstantBuffer cb;
        //���[���h�s���n��
        auto trans = owner()->transform();
        cb.world = trans->getWorldTransform();
        cb.world.transpose();
        //���[���h�A�J�����A�ˉe�s���n��
        cb.WVP = trans->getWorldTransform() * mCamera->getViewProjection();
        cb.WVP.transpose();

        memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
        mShader->unmap(0);
    }

    //�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
    mMesh->getVertexArray()->setVertexBuffer();

    //���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
    mShader->setPSConstantBuffers(1);

    //�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
    for (size_t i = 0; i < mMesh->getNumMaterial(); i++) {
        //�g�p����Ă��Ȃ��}�e���A���΍�
        auto mat = mMesh->getMaterial(i);
        if (mat->numFace == 0) {
            continue;
        }
        //�C���f�b�N�X�o�b�t�@�[���Z�b�g
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

        //�v���~�e�B�u�������_�����O
        Singleton<DirectX>::instance().drawIndexed(mat->numFace * 3);
    }
}

size_t MeshComponent::getNumMaterial() const {
    return mMesh->getNumMaterial();
}

std::shared_ptr<Material> MeshComponent::getMaterial(unsigned index) const {
    return mMesh->getMaterial(index);
}

const Vector3& MeshComponent::getCenter() const {
    return mCenter;
}

float MeshComponent::getRadius() const {
    return mRadius;
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

bool MeshComponent::isDead() {
    return mState == State::DEAD;
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
