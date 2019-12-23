#pragma once

#include "SceneBase.h"
#include <memory>

class Renderer;

class SceneManager {
public:
    SceneManager(std::shared_ptr<Renderer> renderer);
    ~SceneManager();
    void update();
    void draw() const;

private:
    void change(Scene next);

private:
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<SceneBase> mCurrentScene;
};
