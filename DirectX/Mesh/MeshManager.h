#pragma once

#include <memory>
#include <list>

class Mesh;
class Camera;

class MeshManager {
public:
    MeshManager();
    ~MeshManager();
    void update();
    void draw(std::shared_ptr<Camera> camera);
    void add(Mesh* mesh);
    void clear();

private:
    void remove();

    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;

private:
    std::list<std::shared_ptr<Mesh>> mMeshes;
};

