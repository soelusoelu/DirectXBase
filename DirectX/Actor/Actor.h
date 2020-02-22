#pragma once

#include "Transform3D.h"
#include "../Utility/Math.h"
#include <memory>
#include <rapidjson/document.h>

class ActorManager;
class ComponentManager;
class Renderer;
class Time;

class Actor {
    enum class State {
        ACTIVE,
        DEAD
    };

protected:
    Actor(std::shared_ptr<Renderer> renderer, const char* tag = "");
public:
    virtual ~Actor();

    //すべての更新
    void update();
    //アクター固有の更新
    virtual void updateActor() = 0;

    //ワールド行列の更新
    void computeWorldTransform();

    //アクター削除
    void destroy();
    void destroy(float sec);

    //ゲッター、セッター
    std::shared_ptr<Renderer> renderer() const;
    std::shared_ptr<ComponentManager> componentManager() const;
    std::shared_ptr<Transform3D> transform() const;
    bool isDead() const;
    const char* tag() const;

    //ロード/セーブ
    virtual void loadProperties(const rapidjson::Value& inObj) {};
    //virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;

    //指定されたプロパティでアクターを生成
    template <typename T>
    static Actor* create(std::shared_ptr<Renderer> renderer, const rapidjson::Value& inObj) {
        T* t = new T(renderer);
        t->mTransform->loadProperties(inObj);
        t->loadProperties(inObj);
        return t;
    }

    //ActorManagerの登録
    static void setActorManager(ActorManager* manager);
    ActorManager* getActorManager();

private:
    void destroyTimer();

private:
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<ComponentManager> mComponentManager;
    std::shared_ptr<Transform3D> mTransform;
    std::unique_ptr<Time> mDestroyTimer;
    State mState;
    const char* mTag;

    static ActorManager* mActorManager;
};

