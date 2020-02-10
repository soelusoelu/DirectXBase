#include "MeshManager.h"
#include "Mesh.h"
#include "../Device/Renderer.h"

MeshManager::MeshManager() = default;

MeshManager::~MeshManager() = default;

void MeshManager::update() {
    remove();
}

void MeshManager::draw(std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera) const {
    //プリミティブ・トポロジーをセット
    renderer->setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_LIST);

    //各テクスチャ上にレンダリング
    Mesh::renderToTexture(renderer);

    for (const auto& mesh : mMeshes) {
        if (!mesh->getActive() || mesh->isDead()) {
            continue;
        }

        renderer->setRasterizerStateFront();
        mesh->renderMesh(renderer, camera);
        renderer->setRasterizerStateBack();
        mesh->renderMesh(renderer, camera);
    }

    //各テクスチャを参照してレンダリング
    Mesh::renderFromTexture(renderer, camera);
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
        if ((*itr)->isDead()) {
            itr = mMeshes.erase(itr);
        } else {
            ++itr;
        }
    }
}
