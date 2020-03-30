#pragma once

#include <memory>

class GameObjectManager;
class Renderer;
class Camera;

class SceneBase {
    friend class SceneManager;
protected:
    SceneBase();
    virtual ~SceneBase();
public:
    virtual void start() = 0;
    virtual void update() = 0;
    void nextScene(std::shared_ptr<SceneBase> next);

private:
    //SceneManagerがアクセスするよう
    void set(std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera, GameObjectManager* gameObjectManager);
    std::shared_ptr<SceneBase> getNextScene() const;

protected:
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<Camera> mCamera;
    GameObjectManager* mGameObjectManager;

private:
    std::shared_ptr<SceneBase> mNext;
};
