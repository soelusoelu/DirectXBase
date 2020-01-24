#pragma once

#include <memory>

class UIManager;
class SpriteManager;
class Renderer;

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
    UIManager* mUIManager;
    SpriteManager* mSpriteManager;
    std::shared_ptr<Renderer> mRenderer;

private:
    std::shared_ptr<SceneBase> mNext;
    bool mIsStarted;
};
