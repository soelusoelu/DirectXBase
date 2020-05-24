#pragma once

#include "Component.h"
#include "IMesh.h"
#include "../Math/Math.h"
#include "../System/Game.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>
#include <vector>

struct MeshConstantBuffer {
    ALIGN16 Matrix4 world; //ワールド行列
    ALIGN16 Matrix4 WVP; //ワールドから射影までの変換行列
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

class MeshComponent : public Component, public IMesh, public std::enable_shared_from_this<MeshComponent> {
    using MaterialPtr = std::shared_ptr<Material>;
    using MaterialPtrArray = std::vector<MaterialPtr>;

    enum class State {
        ACTIVE,
        NON_ACTIVE,
        DEAD
    };

public:
    MeshComponent();
    virtual ~MeshComponent();
    virtual void start() override;
    virtual void onSetActive(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;

    virtual bool isVisible() const override;
    virtual size_t getNumMaterial() const override;
    virtual std::shared_ptr<Material> getMaterial(unsigned index) const override;
    virtual const Vector3& getCenter() const override;
    virtual float getRadius() const override;
    virtual void setColor(const Vector3& color) override;
    virtual const Vector3& getColor() const override;
    virtual void destroy() override;
    virtual void setActive(bool value) override;
    virtual bool getActive() const override;
    virtual bool isDead() const override;

    virtual void setMesh(const std::string& fileName);
    virtual void setShader();
    virtual void draw();

    static void setMeshManager(MeshManager* manager);

protected:
    void addToManager(bool isTransparent = false);

protected:
    std::shared_ptr<IMeshLoader> mMesh;
    std::shared_ptr<Shader> mShader;
    std::shared_ptr<Camera> mCamera;
    MaterialPtrArray mMaterials;
    State mState;
    Vector3 mCenter;
    float mRadius;
    Vector3 mColor;

    static MeshManager* mMeshManager;
};
