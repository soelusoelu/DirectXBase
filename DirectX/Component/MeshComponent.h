#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include "../System/Game.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

struct MeshConstantBuffer {
    ALIGN16 Matrix4 world; //���[���h�s��
    ALIGN16 Matrix4 WVP; //���[���h����ˉe�܂ł̕ϊ��s��
};

struct MaterialConstantBuffer {
    ALIGN16 Vector4 diffuse;
    ALIGN16 Vector4 specular;
    ALIGN16 float textureFlag;
};

class Camera;
class GameObject;
class IMeshLoader;
class MeshManager;
class Shader;
struct Material;

class MeshComponent : public Component, public std::enable_shared_from_this<MeshComponent> {
    enum class State {
        ACTIVE,
        NON_ACTIVE,
        DEAD
    };

public:
    MeshComponent(std::shared_ptr<GameObject> owner, const std::string& type = "MeshComponent");
    virtual ~MeshComponent();
    virtual void start() override;
    virtual void onSetActive(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    virtual void setMesh(const std::string& fileName);
    virtual void setShader();
    virtual void draw();
    //�}�e���A�����̎擾
    size_t getNumMaterial() const;
    //�}�e���A���̎擾
    std::shared_ptr<Material> getMaterial(unsigned index) const;
    //���S���W�̎擾
    const Vector3& getCenter() const;
    //���a�̎擾
    float getRadius() const;
    //�S�̂̐F����(�V�F�[�_�[���Ŏg�p���Ă���K�v����)
    void setColor(const Vector3& color);
    const Vector3& getColor() const;
    //���
    void destroy();
    void setActive(bool value);
    bool getActive() const;
    bool isDead();

    static void setMeshManager(MeshManager* manager);

protected:
    void addToManager(bool isTransparent = false);

protected:
    std::shared_ptr<IMeshLoader> mMesh;
    std::shared_ptr<Shader> mShader;
    std::shared_ptr<Camera> mCamera;
    State mState;
    Vector3 mCenter;
    float mRadius;
    Vector3 mColor;

    static MeshManager* mMeshManager;
};
