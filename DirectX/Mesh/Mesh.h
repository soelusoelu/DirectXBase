#pragma once

#include "../System/DirectXIncLib.h"
#include "../System/Game.h"
#include "../Utility/Collision.h"
#include "../Utility/Math.h"
#include <memory>
#include <string>
#include <vector>

class Camera;
class FBX;
class MeshLoader;
class MeshManager;
class Renderer;
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

class Mesh : public std::enable_shared_from_this<Mesh> {
public:
    enum class State {
        ACTIVE,
        NON_ACTIVE,
        DEAD
    };

    Mesh(std::shared_ptr<Renderer> renderer, const std::string& fileName);
    ~Mesh();
    void addToManager();
    void createSphere(std::shared_ptr<Sphere>* sphere) const;
    void draw(std::shared_ptr<Camera> camera) const;
    void setTransform(std::shared_ptr<Transform3D> transform);
    std::shared_ptr<MeshLoader> getMeshData() const;
    void destroy();
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;

    static void setMeshManager(MeshManager* manager);

private:
    //std::shared_ptr<MeshLoader> mLoader;
    std::shared_ptr<FBX> mFBX;
    std::shared_ptr<Transform3D> mTransform;
    std::shared_ptr<Shader> mShader;
    State mState;

    static MeshManager* mMeshManager;
};
