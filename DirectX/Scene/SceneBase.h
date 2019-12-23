#pragma once

#include <memory>

enum class Scene {
    GAME_PLAY,

    NONE
};

class ActorManager;
class UIManager;
class SpriteManager;
class Renderer;

class SceneBase {
protected:
    SceneBase(std::shared_ptr<Renderer> renderer);
    virtual ~SceneBase();
public:
    void update();
    virtual void updateScene() = 0;
    void draw();
    void next(Scene next);
    Scene getNextScene() const;

protected:
    ActorManager* mActorManager;
    //std::unique_ptr<UIManager> mUIManager;
    SpriteManager* mSpriteManager;
    std::shared_ptr<Renderer> mRenderer;

private:
    Scene mNext;
};
