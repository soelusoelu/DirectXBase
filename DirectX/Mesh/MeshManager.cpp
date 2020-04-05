#include "MeshManager.h"
#include "Mesh.h"
#include "../Device/Renderer.h"
#include "../System/BlendDesc.h"
#include "../System/BlendState.h"
#include "../System/DepthStencilState.h"
#include "../System/DirectX.h"
#include "../System/RasterizerDesc.h"
#include "../System/RasterizerState.h"

MeshManager::MeshManager() = default;

MeshManager::~MeshManager() = default;

void MeshManager::update() {
    remove();
}

void MeshManager::draw(const Camera& camera) const {
    if (mMeshes.empty()) {
        return;
    }

    //プリミティブ・トポロジーをセット
    Singleton<DirectX>::instance().setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_LIST);

    for (const auto& mesh : mMeshes) {
        if (!mesh->getActive() || mesh->isDead()) {
            continue;
        }

        RasterizerDesc rd;

        rd.cullMode = CullMode::FRONT;
        Singleton<DirectX>::instance().rasterizerState()->setRasterizerState(rd);
        mesh->draw(camera);

        rd.cullMode = CullMode::BACK;
        Singleton<DirectX>::instance().rasterizerState()->setRasterizerState(rd);
        mesh->draw(camera);
    }
}

void MeshManager::drawTransparent(const Camera& camera, const DirectionalLight& dirLight) const {
    if (mTransparentMeshes.empty()) {
        return;
    }

    //プリミティブ・トポロジーをセット
    Singleton<DirectX>::instance().setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_LIST);
    //デプステスト有効化
    Singleton<DirectX>::instance().depthStencilState()->depthTest(true);
    //デプスマスク有効化
    Singleton<DirectX>::instance().depthStencilState()->depthMask(true);
    //通常合成
    BlendDesc bd;
    bd.renderTarget.srcBlend = Blend::SRC_ALPHA;
    bd.renderTarget.destBlend = Blend::INV_SRC_ALPHA;
    Singleton<DirectX>::instance().blendState()->setBlendState(bd);

    for (const auto& mesh : mTransparentMeshes) {
        if (!mesh->getActive() || mesh->isDead()) {
            continue;
        }

        RasterizerDesc rd;

        rd.cullMode = CullMode::FRONT;
        Singleton<DirectX>::instance().rasterizerState()->setRasterizerState(rd);
        mesh->drawTransparent(camera, dirLight);

        rd.cullMode = CullMode::BACK;
        Singleton<DirectX>::instance().rasterizerState()->setRasterizerState(rd);
        mesh->drawTransparent(camera, dirLight);
    }
}

void MeshManager::add(const MeshPtr& mesh) {
    mMeshes.emplace_back(mesh);
}

void MeshManager::addTransparent(const MeshPtr& mesh) {
    mTransparentMeshes.emplace_back(mesh);
}

void MeshManager::clear() {
    mMeshes.clear();
    mTransparentMeshes.clear();
}

void MeshManager::remove() {
    auto itr = mMeshes.begin();
    while (itr != mMeshes.end()) {
        if ((*itr)->isDead()) {
            itr = mMeshes.erase(itr);
        } else {
            ++itr;
        }
    }
    itr = mTransparentMeshes.begin();
    while (itr != mTransparentMeshes.end()) {
        if ((*itr)->isDead()) {
            itr = mTransparentMeshes.erase(itr);
        } else {
            ++itr;
        }
    }
}
