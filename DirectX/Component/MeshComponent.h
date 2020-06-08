#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include "../System/GlobalFunction.h"
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

class MeshComponent : public Component, public std::enable_shared_from_this<MeshComponent> {
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
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

    //カメラに写っているか
    bool isVisible() const;
    //マテリアル数の取得
    size_t getNumMaterial() const;
    //マテリアルの取得
    const MaterialPtr& getMaterial(unsigned index) const;
    //中心座標の取得
    const Vector3& getCenter() const;
    //半径の取得
    float getRadius() const;
    //全体の色合い(シェーダー側で使用している必要あり)
    void setColor(const Vector3& color);
    const Vector3& getColor() const;
    //状態
    void destroy();
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;

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
