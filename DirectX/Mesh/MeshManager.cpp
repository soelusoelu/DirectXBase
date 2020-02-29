#include "MeshManager.h"
#include "Mesh.h"
#include "../Device/Renderer.h"
#include "../System/DirectX.h"
#include "../System/RasterizerDesc.h"
#include "../System/RasterizerState.h"

MeshManager::MeshManager() = default;

MeshManager::~MeshManager() = default;

void MeshManager::update() {
    remove();
}

void MeshManager::draw(std::shared_ptr<Camera> camera) const {
    if (mMeshes.empty()) {
        return;
    }

    //プリミティブ・トポロジーをセット
    Singleton<DirectX>::instance().setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_STRIP);

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

void MeshManager::add(std::shared_ptr<Mesh> mesh) {
    mMeshes.emplace_back(mesh);
}

void MeshManager::clear() {
    mMeshes.clear();
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
}
