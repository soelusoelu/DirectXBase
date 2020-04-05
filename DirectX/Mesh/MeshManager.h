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
    void draw(const Camera& camera) const;
    void drawTransparent(const Camera& camera, const DirectionalLight& dirLight) const;
    void add(const MeshPtr& mesh);
    void addTransparent(const MeshPtr& mesh);
    void clear();

private:
    void remove();

    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;

private:
    MeshPtrList mMeshes;
    MeshPtrList mTransparentMeshes;
};

