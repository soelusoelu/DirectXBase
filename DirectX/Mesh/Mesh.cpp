#include "Mesh.h"
#include "MeshManager.h"
#include "../Actor/Transform3D.h"
#include "../Device/Renderer.h"
#include "../Shader/Shader.h"
#include "../System/Buffer.h"
#include "../System/BufferDesc.h"
#include "../System/Direct3D11.h"
#include "../System/DirectXIncLib.h"
#include "../System/Game.h"
#include "../System/InputElement.h"
#include "../System/SubResourceDesc.h"
#include "../System/VertexStreamDesc.h"
#include <fstream>
#include <string>
#include <stdio.h>

Mesh::Mesh(std::shared_ptr<Renderer> renderer, const char* fileName) :
    mRenderer(renderer),
    mTransform(nullptr),
    mShader(renderer->createShader("Mesh.hlsl")),
    mMaterials(0),
    mNumVert(0),
    mNumNormal(0),
    mNumTex(0),
    mNumFace(0),
    mNumMaterial(0),
    mVertices(0),
    mNormals(0),
    mTextures(0),
    mVertexBuffer(nullptr),
    mIndexBuffers(0),
    mState(MeshState::ACTIVE) {
    if (!loadMesh(fileName)) {
        MSG(L"���b�V���쐬���s");
        return;
    }

    //���b�V���p�R���X�^���g�o�b�t�@�̍쐬
    mShader->createConstantBuffer(sizeof(MeshShaderConstantBuffer0), 0);
    mShader->createConstantBuffer(sizeof(MeshShaderConstantBuffer1), 1);

    //�C���v�b�g���C�A�E�g�̐���
    constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 6, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    mShader->createInputLayout(layout, numElements);

    if (mMeshManager) {
        mMeshManager->add(this);
    }
}

Mesh::~Mesh() = default;

void Mesh::createSphere(std::shared_ptr<Sphere> sphere) const {
    //�o�E���f�B���O�X�t�B�A�쐬
    //D3DXVECTOR3 center;
    //D3DXVECTOR3 first(mCoord->x, mCoord->y, mCoord->z);
    //D3DXComputeBoundingSphere(&first, mNumVert, sizeof(D3DXVECTOR3), &center, &sphere->radius);
    //sphere->center.x = center.x;
    //sphere->center.y = center.y;
    //sphere->center.z = center.z;
}

void Mesh::draw() const {
    //�v���~�e�B�u�E�g�|���W�[���Z�b�g
    mRenderer->setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_STRIP);

    mRenderer->deviceContext()->RSSetState(mRenderer->rasterizerState());
    rendererMesh();
    mRenderer->deviceContext()->RSSetState(mRenderer->rasterizerStateBack());
    rendererMesh();
}

void Mesh::setTransform(std::shared_ptr<Transform3D> transform) {
    mTransform = transform;
}

MeshState Mesh::getState() const {
    return mState;
}

void Mesh::setMeshManager(MeshManager * manager) {
    mMeshManager = manager;
}

bool Mesh::loadMesh(const char* fileName) {
    //���O�ɒ��_���Ȃǂ𒲂ׂ�
    if (!tempLoad(fileName)) {
        MSG(L"Mesh�t�@�C���̎��O�ǂݍ��ݎ��s");
        return false;
    }

    //�T�C�Y�ύX
    auto* meshVertices = new MeshVertex[mNumVert];
    mVertices.resize(mNumVert);
    mNormals.resize(mNumNormal);
    mTextures.resize(mNumTex);

    //OBJ�t�@�C�����J���ē��e��ǂݍ���
    setOBJDirectory();
    std::ifstream ifs(fileName, std::ios::in);

    std::string line;
    std::string strip;
    unsigned vCount = 0;
    unsigned vnCount = 0;
    unsigned vtCount = 0;
    float x, y, z;

    //�{�ǂݍ���
    while (!ifs.eof()) {
        //�L�[���[�h�ǂݍ���
        std::getline(ifs, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        //�󔒂�����܂œǂݍ���
        strip = stringStrip(line, ' ');

        //���_�ǂݍ���
        if (strip == "v") {
            auto sub = line.substr(2); //�uv �v�̕�������
            sscanf_s(sub.c_str(), "%f %f %f", &x, &y, &z);
            mVertices[vCount].x = -x;
            mVertices[vCount].y = y;
            mVertices[vCount].z = z;
            vCount++;
        }

        //�@���ǂݍ���
        if (strip == "vn") {
            auto sub = line.substr(3); //�uvn �v�̕�������
            sscanf_s(sub.c_str(), "%f %f %f", &x, &y, &z);
            mNormals[vnCount].x = -x;
            mNormals[vnCount].y = y;
            mNormals[vnCount].z = z;
            vnCount++;
        }

        //�e�N�X�`���[���W �ǂݍ���
        if (strip == "vt") {
            auto sub = line.substr(3); //�uvt �v�̕�������
            sscanf_s(sub.c_str(), "%f %f", &x, &y);
            mTextures[vtCount].x = x;
            mTextures[vtCount].y = -y; //OBJ�t�@�C����Y�������t�Ȃ̂ō��킹��
            vtCount++;
        }
    }

    //�}�e���A���̐������C���f�b�N�X�o�b�t�@�[���쐬
    mIndexBuffers.resize(mNumMaterial);

    //�t�F�C�X�ǂݍ��� �o���o���Ɏ��^����Ă���\��������̂ŁA�}�e���A�����𗊂�ɂȂ����킹��
    bool matFlag = false;
    int* faceBuffer = new int[mNumFace * 3]; //3���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)

    int v1 = 0, v2 = 0, v3 = 0;
    int vn1 = 0, vn2 = 0, vn3 = 0;
    int vt1 = 0, vt2 = 0, vt3 = 0;
    unsigned fCount = 0;

    for (unsigned i = 0; i < mNumMaterial; i++) {
        ifs.clear();
        ifs.seekg(0, std::ios_base::beg);
        fCount = 0;

        while (!ifs.eof()) {
            //�L�[���[�h �ǂݍ���
            std::getline(ifs, line);

            if (line.empty() || line[0] == '#') {
                continue;
            }

            //�󔒂�����܂œǂݍ���
            strip = stringStrip(line, ' ');

            //�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
            if (strip == "usemtl") {
                auto mat = line.substr(7); //�uusemtl �v�̕�������
                matFlag = (mMaterials[i].matName == mat);
            }

            if (strip == "f" && matFlag) {
                auto sub = line.substr(2); //�uf �v�̕�������
                if (mMaterials[i].texture) { //�e�N�X�`���[����T�[�t�F�C�X
                    sscanf_s(sub.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
                } else { //�e�N�X�`���[�����T�[�t�F�C�X
                    sscanf_s(sub.c_str(), "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
                }

                //�e�N�X�`���[���W > ���_�������肦��
                if (vtCount > vCount) {
                    //�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X�� �e�N�X�`���[���W�C���f�b�N�X����ɂ���(�e�N�X�`���[���W >= ���_���W�Ȃ̂�)
                    sscanf_s(sub.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
                    faceBuffer[fCount * 3] = vt1 - 1;
                    faceBuffer[fCount * 3 + 1] = vt2 - 1;
                    faceBuffer[fCount * 3 + 2] = vt3 - 1;
                    fCount++;
                    //���_�\���̂ɑ�� �e�N�X�`���[���W�C���f�b�N�X����ɂ���(�e�N�X�`���[���W >= ���_���W�Ȃ̂�)
                    meshVertices[vt1 - 1].pos = mVertices[v1 - 1];
                    meshVertices[vt1 - 1].norm = mNormals[vn1 - 1];
                    meshVertices[vt1 - 1].tex = mTextures[vt1 - 1];
                    meshVertices[vt2 - 1].pos = mVertices[v2 - 1];
                    meshVertices[vt2 - 1].norm = mNormals[vn2 - 1];
                    meshVertices[vt2 - 1].tex = mTextures[vt2 - 1];
                    meshVertices[vt3 - 1].pos = mVertices[v3 - 1];
                    meshVertices[vt3 - 1].norm = mNormals[vn3 - 1];
                    meshVertices[vt3 - 1].tex = mTextures[vt3 - 1];
                } else {
                    //�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
                    sscanf_s(sub.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
                    faceBuffer[fCount * 3] = v1 - 1;
                    faceBuffer[fCount * 3 + 1] = v2 - 1;
                    faceBuffer[fCount * 3 + 2] = v3 - 1;
                    fCount++;
                    //���_�\���̂ɑ��
                    meshVertices[v1 - 1].pos = mVertices[v1 - 1];
                    meshVertices[v1 - 1].norm = mNormals[vn1 - 1];
                    meshVertices[v1 - 1].tex = mTextures[vt1 - 1];
                    meshVertices[v2 - 1].pos = mVertices[v2 - 1];
                    meshVertices[v2 - 1].norm = mNormals[vn2 - 1];
                    meshVertices[v2 - 1].tex = mTextures[vt2 - 1];
                    meshVertices[v3 - 1].pos = mVertices[v3 - 1];
                    meshVertices[v3 - 1].norm = mNormals[vn3 - 1];
                    meshVertices[v3 - 1].tex = mTextures[vt3 - 1];
                }
            }
        }
        if (fCount == 0) { //�g�p����Ă��Ȃ��}�e���A���΍�
            mIndexBuffers[i] = nullptr;
            continue;
        }

        //�C���f�b�N�X�o�b�t�@�[���쐬
        BufferDesc bd;
        bd.size = sizeof(int) * fCount * 3;
        bd.usage = BufferUsage::BUFFER_USAGE_DEFAULT;
        bd.type = BufferType::BUFFER_TYPE_INDEX;
        SubResourceDesc sub;
        sub.data = faceBuffer;

        mIndexBuffers[i] = mRenderer->createBuffer(bd, &sub);

        mMaterials[i].numFace = fCount;
    }

    delete[] faceBuffer;

    //�o�[�e�b�N�X�o�b�t�@�[���쐬
    BufferDesc bd;
    bd.size = sizeof(MeshVertex) * vCount;
    bd.usage = BufferUsage::BUFFER_USAGE_DEFAULT;
    bd.type = BufferType::BUFFER_TYPE_VERTEX;
    SubResourceDesc sub;
    sub.data = meshVertices;

    mVertexBuffer = mRenderer->createBuffer(bd, &sub);

    delete[] meshVertices;

    return true;
}

bool Mesh::tempLoad(const char* fileName) {
    //OBJ�t�@�C�����J���ē��e��ǂݍ���
    setOBJDirectory();
    std::ifstream ifs(fileName, std::ios::in);
    if (ifs.fail()) {
        MSG(L"OBJ�t�@�C�������݂��܂���");
        return false;
    }

    std::string line;
    std::string strip;
    //���O�ɒ��_���A�|���S�����𒲂ׂ�
    while (!ifs.eof()) {
        //�L�[���[�h�ǂݍ���
        std::getline(ifs, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        //�󔒂�����܂œǂݍ���
        strip = stringStrip(line, ' ');

        //�}�e���A���ǂݍ���
        if (strip == "mtllib") {
            auto mat = line.substr(7); //�umtllib �v�̕�������
            loadMaterial(mat.c_str(), &mMaterials);
        }
        //���_
        if (strip == "v") {
            mNumVert++;
        }
        //�@��
        if (strip == "vn") {
            mNumNormal++;
        }
        //�e�N�X�`���[���W
        if (strip == "vt") {
            mNumTex++;
        }
        //�t�F�C�X(�|���S��)
        if (strip == "f") {
            mNumFace++;
        }
    }
    //�e�N�X�`���[���W > ���_�������肦��B���̏ꍇ�A���_�𑝂₷�K�v������
    if (mNumTex > mNumVert) {
        mNumVert = mNumTex;
    }

    return true;
}

bool Mesh::loadMaterial(const char* fileName, std::vector<Material> * material) {
    //�}�e���A���t�@�C�����J���ē��e��ǂݍ���
    std::ifstream ifs(fileName, std::ios::in);
    if (ifs.fail()) {
        MSG(L"mtl�t�@�C�������݂��܂���");
        return false;
    }

    //�}�e���A�����𒲂ׂ�
    mNumMaterial = 0;
    std::string line;
    while (!ifs.eof()) {
        //�L�[���[�h�ǂݍ���
        std::getline(ifs, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        auto strip = stringStrip(line, ' ');

        //�}�e���A����
        if (strip == "newmtl") {
            mNumMaterial++;
        }
    }
    material->resize(mNumMaterial);
    std::vector<Material> materials(mNumMaterial);

    //�{�ǂݍ���
    ifs.clear();
    ifs.seekg(0, std::ios_base::beg);
    Vector4 v(0.f, 0.f, 0.f, 1.f);
    int matCount = -1;

    while (!ifs.eof()) {
        //�L�[���[�h�ǂݍ���
        std::getline(ifs, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        auto strip = stringStrip(line, ' ');

        //�}�e���A����
        if (strip == "newmtl") {
            matCount++;
            materials[matCount].matName = line.substr(7); //�unewmtl �v�̕�������
        }
        //Ka �A���r�G���g
        if (strip == "Ka") {
            auto sub = line.substr(3); //�uKa �v�̕�������
            sscanf_s(sub.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
            materials[matCount].Ka = v;
        }
        //Kd �f�B�t���[�Y
        if (strip == "Kd") {
            auto sub = line.substr(3); //�uKd �v�̕�������
            auto i = sscanf_s(sub.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
            materials[matCount].Kd = v;
        }
        //Ks �X�y�L�����[
        if (strip == "Ks") {
            auto sub = line.substr(3); //�uKs �v�̕�������
            sscanf_s(sub.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
            materials[matCount].Ks = v;
        }
        //map_Kd �e�N�X�`���[
        if (strip == "map_Kd") {
            materials[matCount].textureName = line.substr(7); //�umap_Kd �v�̕�������
            //�e�N�X�`���[���쐬
            if (FAILED(D3DX11CreateShaderResourceViewFromFileA(mRenderer->device(), materials[matCount].textureName.c_str(), nullptr, nullptr, &materials[matCount].texture, nullptr))) {
                MSG(L"Mesh�̃e�N�X�`�������݂��܂���");
                return false;
            }
            D3D11_SAMPLER_DESC sd;
            ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
            sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
            sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
            sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
            mRenderer->device()->CreateSamplerState(&sd, &materials[matCount].sampleLinear);
        }
    }

    material->swap(materials);

    return true;
}

void Mesh::rendererMesh() const {
    //�g�p����V�F�[�_�[�̓o�^
    mShader->setVSShader();
    mShader->setPSShader();
    //���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
    mShader->setVSConstantBuffers(0);
    mShader->setPSConstantBuffers(0);
    //���_�C���v�b�g���C�A�E�g���Z�b�g
    mShader->setInputLayout();

    //�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
    D3D11_MAPPED_SUBRESOURCE pData;
    if (SUCCEEDED(mRenderer->deviceContext()->Map(mShader->getConstantBuffer(0)->buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
        MeshShaderConstantBuffer0 cb;
        //���[���h�s���n��
        cb.world = mTransform->getWorldTransform();
        cb.world.transpose();
        //���[���h�A�J�����A�ˉe�s���n��
        cb.WVP = mTransform->getWorldTransform() /** Singleton<Camera>::instance().getView() * Singleton<Camera>::instance().getProjection()*/;
        cb.WVP.transpose();
        //���C�g�̕�����n��
        cb.lightDir = Vector4(1.f, -1.f, 1.f, 0.0f);
        //���_�ʒu��n��
        //sg.eye = Vector4(Singleton<Camera>::instance().getPosition(), 0);

        memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(cb));
        mRenderer->deviceContext()->Unmap(mShader->getConstantBuffer(0)->buffer(), 0);
    }

    //�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
    VertexStreamDesc stream;
    stream.sharedBuffer = mVertexBuffer;
    stream.offset = 0;
    stream.stride = sizeof(MeshVertex);
    mRenderer->setVertexBuffer(&stream);

    //�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
    for (unsigned i = 0; i < mNumMaterial; i++) {
        //�g�p����Ă��Ȃ��}�e���A���΍�
        if (mMaterials[i].numFace == 0) {
            continue;
        }
        //�C���f�b�N�X�o�b�t�@�[���Z�b�g
        stream.stride = sizeof(int);
        stream.offset = 0;
        mRenderer->setIndexBuffer(mIndexBuffers[i]);

        //�}�e���A���̊e�v�f���G�t�F�N�g(�V�F�[�_�[)�ɓn��
        D3D11_MAPPED_SUBRESOURCE pData;
        if (SUCCEEDED(mRenderer->deviceContext()->Map(mShader->getConstantBuffer(1)->buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
            MeshShaderConstantBuffer1 sg;
            sg.ambient = mMaterials[i].Ka; //�A���r�G���g�����V�F�[�_�[�ɓn��
            sg.diffuse = mMaterials[i].Kd; //�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
            sg.specular = mMaterials[i].Ks; //�X�y�L�����[���V�F�[�_�[�ɓn��

            //���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
            mShader->setVSConstantBuffers(1);
            mShader->setVSConstantBuffers(1);

            //�e�N�X�`���[���V�F�[�_�[�ɓn��
            if (mMaterials[i].texture) {
                mRenderer->deviceContext()->PSSetShaderResources(0, 1, &mMaterials[i].texture);
                mRenderer->deviceContext()->PSSetSamplers(0, 1, &mMaterials[i].sampleLinear);
                sg.texture.x = 1;
            } else {
                sg.texture.x = 0;
            }

            memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(sg));
            mRenderer->deviceContext()->Unmap(mShader->getConstantBuffer(1)->buffer(), 0);
        }
        //�v���~�e�B�u�������_�����O
        mRenderer->drawIndexed(mMaterials[i].numFace * 3);
    }
}

std::string Mesh::stringStrip(const std::string & string, const char delimiter) {
    std::string temp = "";
    for (const auto& s : string) {
        if (s != delimiter) {
            temp += s;
        } else {
            break;
        }
    }
    return temp;
}

MeshManager* Mesh::mMeshManager = nullptr;
