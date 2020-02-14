#pragma once

#include <memory>

class Camera;
class MeshManager;
class Renderer;
class SceneBase;
class SpriteManager;
class UIManager;

class SceneManager {
public:
    SceneManager(std::shared_ptr<Renderer> renderer);
    ~SceneManager();
    void update();
    void draw() const;

private:
    void change();

private:
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<SceneBase> mCurrentScene;
    std::shared_ptr<Camera> mCamera;
    MeshManager* mMeshManager;
    SpriteManager* mSpriteManager;
    UIManager* mUIManager;
};
