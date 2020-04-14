#pragma once

#include <memory>

class GameObjectManager;

class SceneBase {
    friend class SceneManager;
protected:
    SceneBase();
    virtual ~SceneBase();
public:
    virtual void start() {};
    virtual void update() {};
    void nextScene(std::shared_ptr<SceneBase> next);

private:
    //SceneManagerがアクセスするよう
    void set(GameObjectManager* gameObjectManager);
    std::shared_ptr<SceneBase> getNextScene() const;

protected:
    GameObjectManager* mGameObjectManager;

private:
    std::shared_ptr<SceneBase> mNext;
};
