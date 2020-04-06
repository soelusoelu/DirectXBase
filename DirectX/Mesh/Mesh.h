#pragma once

#include "IMeshLoader.h"
#include "../Math/Math.h"
#include "../System/DirectXIncLib.h"
#include "../System/Game.h"
#include "../Utility/Collision.h"
#include <memory>
#include <string>
#include <vector>

class Camera;
class DirectionalLight;
class MeshManager;
class Transform3D;
class Shader;

struct MeshConstantBuffer {
    ALIGN16 Matrix4 world; //ワールド行列
    ALIGN16 Matrix4 WVP; //ワールドから射影までの変換行列
};

struct MaterialConstantBuffer {
    ALIGN16 Vector4 diffuse;
    ALIGN16 Vector4 specular;
    ALIGN16 float textureFlag;
};

struct TransparentConstantBuffer {
    ALIGN16 Matrix4 world; //ワールド行列
    ALIGN16 Matrix4 WVP; //ワールドから射影までの変換行列
    ALIGN16 Vector3 lightDir; //ワールド行列
    ALIGN16 Vector3 cameraPos; //ワールドから射影までの変換行列
};

class Mesh : public std::enable_shared_from_this<Mesh> {
public:
    enum class State {
        ACTIVE,
        NON_ACTIVE,
        DEAD
    };

    Mesh(const std::string& fileName);
    ~Mesh();
    void addToManager(bool isTransparent = false);
    void draw(const Camera& camera) const;
    void drawTransparent(const Camera& camera, const DirectionalLight& dirLight) const;
    void setTransform(const std::shared_ptr<Transform3D>& transform);
    std::shared_ptr<IMeshLoader> getMeshData() const;
    //半径の取得
    float getRadius() const;
    //中心座標の取得
    const Vector3& getCenter() const;
    //メッシュの破棄
    void destroy();
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;
    //全体的な色
    void setColor(const Vector3& color);
    const Vector3& getColor() const;
    //アルファ値 (TransparentMeshComponentのみ)
    void setAlpha(float alpha);
    float getAlpha() const;

    static void setMeshManager(MeshManager* manager);

private:
    std::shared_ptr<IMeshLoader> mMesh;
    std::shared_ptr<Transform3D> mTransform;
    std::shared_ptr<Shader> mShader;
    std::shared_ptr<Shader> mShaderTransparent;
    State mState;
    float mRadius;
    Vector3 mCenter;
    Vector3 mColor;
    float mAlpha;

    static MeshManager* mMeshManager;
};
