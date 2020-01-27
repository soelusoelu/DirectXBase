#pragma once

#include "../System/DirectXIncLib.h"
#include "../System/Game.h"
#include "../Utility/Collision.h"
#include "../Utility/Math.h"
#include <list>
#include <memory>
#include <string>
#include <vector>

class Buffer;
class Camera;
class MeshManager;
class Renderer;
class Texture;
class Transform3D;
class Shader;

enum class MeshState {
    ACTIVE,
    NON_ACTIVE,
    DEAD
};

struct MeshShaderConstantBuffer0 {
    Matrix4 world; //���[���h�s��
    Matrix4 WVP; //���[���h����ˉe�܂ł̕ϊ��s��
    Vector4 lightDir; //���C�g����
    Vector4 eye; //�J�����ʒu
};

struct MeshShaderConstantBuffer1 {
    Vector4 ambient; //�A���r�G���g��
    Vector4 diffuse; //�f�B�t���[�Y�F
    Vector4 specular; //���ʔ���
    Vector4 texture; //�e�N�X�`���[���\���Ă��郁�b�V�����ǂ����̃t���O
};

//���_�̍\����
struct MeshVertex {
    Vector3 pos;
    Vector3 norm;
    Vector2 tex;
};

struct Material {
    std::string matName; //newmtl
    Vector4 Ka; //�A���r�G���g
    Vector4 Kd; //�f�B�t���[�Y
    Vector4 Ks; //�X�y�L�����[
    std::string textureName; //�e�N�X�`���[�t�@�C����
    ID3D11ShaderResourceView* texture;
    ID3D11SamplerState* sampleLinear;
    unsigned numFace; //���̃}�e���A���ł���|���S����
    Material() {
        ZeroMemory(this, sizeof(Material));
    }
    ~Material() {
        SAFE_RELEASE(texture);
        SAFE_RELEASE(sampleLinear);
    }
};

class Mesh {
public:
    Mesh(std::shared_ptr<Renderer> renderer, const char* fileName);
    ~Mesh();
    void createSphere(std::shared_ptr<Sphere> sphere) const;
    static void drawAll(std::list<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera);
    void setTransform(std::shared_ptr<Transform3D> transform);
    void destroy();
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;

    static void setMeshManager(MeshManager* manager);

private:
    bool loadMesh(std::shared_ptr<Renderer> renderer, const char* fileName);
    bool tempLoad(std::shared_ptr<Renderer> renderer, const char* fileName); //���O�ɒ��_���Ȃǂ𒲂ׂ�
    bool loadMaterial(std::shared_ptr<Renderer> renderer, const char* fileName, std::vector<Material>* material);
    void rendererMesh(std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera) const;
    std::string stringStrip(const std::string& string, const char delimiter);

private:
    std::shared_ptr<Transform3D> mTransform;
    std::shared_ptr<Shader> mShader;
    std::vector<Material> mMaterials;

    unsigned mNumVert; //���_��
    unsigned mNumNormal; //�@����
    unsigned mNumTex; //�e�N�X�`�����W��
    unsigned mNumFace; //�|���S����
    unsigned mNumMaterial; //�}�e���A����

    std::vector<Vector3> mVertices; //���_���
    std::vector<Vector3> mNormals; //�@�����
    std::vector<Vector2> mTextures; //�e�N�X�`�����W���

    std::shared_ptr<Buffer> mVertexBuffer;
    std::vector<std::shared_ptr<Buffer>> mIndexBuffers;

    MeshState mState;

    static MeshManager* mMeshManager;
};
