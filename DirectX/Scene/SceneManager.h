#pragma once

#include <rapidjson/document.h>
#include <memory>

class Camera;
class DirectionalLight;
class GameObjectManager;
class MeshManager;
class Physics;
class Renderer;
class SceneBase;
class SpriteManager;

class SceneManager {
public:
    SceneManager(std::shared_ptr<Renderer> renderer);
    ~SceneManager();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void update();
    void draw() const;

private:
    void change();

private:
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<SceneBase> mCurrentScene;
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<DirectionalLight> mDirectionalLight;
    GameObjectManager* mGameObjectManager;
    MeshManager* mMeshManager;
    SpriteManager* mSpriteManager;
    Physics* mPhysics;
};
