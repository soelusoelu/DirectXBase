#pragma once

#include <memory>

class Renderer;
class SceneBase;

class SceneManager {
public:
    SceneManager(std::shared_ptr<Renderer> renderer);
    ~SceneManager();
    void update();
    void draw() const;

private:
    void setRendererToScene();

private:
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<SceneBase> mCurrentScene;
};
