#pragma once

#include <memory>
#include <list>

class Mesh;
class Camera;
class DirectionalLight;

class MeshManager {
    using MeshPtr = std::shared_ptr<Mesh>;
    using MeshPtrList = std::list<MeshPtr>;

public:
    MeshManager();
    ~MeshManager();
    void update();
    void draw(std::shared_ptr<Camera> camera) const;
    void drawTransparent(std::shared_ptr<Camera> camera, std::shared_ptr<DirectionalLight> dirLight) const;
    void add(MeshPtr mesh);
    void addTransparent(MeshPtr mesh);
    void clear();

private:
    void remove();

    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;

private:
    MeshPtrList mMeshes;
    MeshPtrList mTransparentMeshes;
};

