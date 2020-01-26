#pragma once

#include <memory>

class Renderer;
class MeshManager;
class SpriteManager;
class UIManager;
class Camera;

class SceneBase {
protected:
    SceneBase();
    virtual ~SceneBase();
public:
    virtual void startScene() = 0;
    virtual void updateScene() = 0;
    void update();
    void draw();
    void nextScene(std::shared_ptr<SceneBase> next);
    std::shared_ptr<SceneBase> getNextScene() const;
    void setRenderer(std::shared_ptr<Renderer> renderer);

private:
    void start();

protected:
    std::shared_ptr<Renderer> mRenderer;
    MeshManager* mMeshManager;
    SpriteManager* mSpriteManager;
    UIManager* mUIManager;
    std::shared_ptr<Camera> mCamera;

private:
    std::shared_ptr<SceneBase> mNext;
    bool mIsStarted;
};
