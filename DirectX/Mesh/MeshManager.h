#pragma once

#include <memory>
#include <list>

class MeshComponent;
class Camera;
class DirectionalLight;

class MeshManager {
    using MeshPtr = std::shared_ptr<MeshComponent>;
    using MeshPtrList = std::list<MeshPtr>;

public:
    MeshManager();
    ~MeshManager();
    void update();
    void draw() const;
    void drawTransparent() const;
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

