﻿#pragma once

#include <rapidjson/document.h>
#include <memory>

class Camera;
class GameObjectManager;
class MeshManager;
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
    GameObjectManager* mGameObjectManager;
    MeshManager* mMeshManager;
    SpriteManager* mSpriteManager;
};
