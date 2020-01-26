#include "MeshManager.h"
#include "Mesh.h"

MeshManager::MeshManager() :
    mMeshes(0) {
}

MeshManager::~MeshManager() = default;

void MeshManager::update() {
    remove();
}

void MeshManager::draw(std::shared_ptr<Camera> camera) {
    for (const auto& mesh : mMeshes) {
        mesh->draw(camera);
    }
}

void MeshManager::add(Mesh* mesh) {
    mMeshes.emplace_back(mesh);
}

void MeshManager::clear() {
    mMeshes.clear();
}

void MeshManager::remove() {
    auto itr = mMeshes.begin();
    while (itr != mMeshes.end()) {
        if ((*itr)->getState() == MeshState::DEAD) {
            itr = mMeshes.erase(itr);
        } else {
            ++itr;
        }
    }
}
