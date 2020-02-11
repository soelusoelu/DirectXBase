#pragma once

#include "../System/DirectXIncLib.h"
#include "../System/Game.h"
#include "../Utility/Collision.h"
#include "../Utility/Math.h"
#include <memory>
#include <string>
#include <vector>

class Camera;
class MeshLoader;
class MeshManager;
class Renderer;
class Transform3D;
class Shader;

struct MeshShaderConstantBuffer0 {
    ALIGN16 Matrix4 world; //ワールド行列
    ALIGN16 Matrix4 WVP; //ワールドから射影までの変換行列
    //ALIGN16 Vector3 lightPos; //ライト位置
    //ALIGN16 Matrix4 lightDir; //ライト方向
    //ALIGN16 Vector3 eye; //カメラ位置
};

//struct MeshShaderConstantBuffer1 {
//    ALIGN16 Vector4 ambient; //アンビエント光
//    ALIGN16 Vector4 diffuse; //ディフューズ色
//    ALIGN16 Vector4 specular; //鏡面反射
//    ALIGN16 float texture; //テクスチャーが貼られているメッシュかどうかのフラグ
//};


class Mesh {
public:
    enum class State {
        ACTIVE,
        NON_ACTIVE,
        DEAD
    };

    Mesh(std::shared_ptr<Renderer> renderer, const std::string& fileName);
    ~Mesh();
    void createSphere(std::shared_ptr<Sphere>* sphere) const;
    void draw(std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera) const;
    void setTransform(std::shared_ptr<Transform3D> transform);
    void destroy();
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;

    static void setMeshManager(MeshManager* manager);

private:
    std::shared_ptr<MeshLoader> mLoader;
    std::shared_ptr<Transform3D> mTransform;
    std::shared_ptr<Shader> mShader;
    State mState;

    static MeshManager* mMeshManager;
};
