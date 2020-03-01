#pragma once

#include "../Utility/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class ActorManager;
class ComponentManager;
class Renderer;
class Time;
class Transform3D;

class Actor : public std::enable_shared_from_this<Actor> {
    enum class State {
        ACTIVE,
        DEAD
    };

protected:
    Actor(std::shared_ptr<Renderer> renderer, const std::string& tag);
public:
    virtual ~Actor();

    //初期化用
    virtual void start() = 0;
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
    const std::string& tag() const;

    //ロード/セーブ
    virtual void loadProperties(const rapidjson::Value& inObj);
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const;

    //指定されたプロパティでアクターを生成
    template <typename T>
    static std::shared_ptr<Actor> create(std::shared_ptr<Renderer> renderer, const rapidjson::Value& inObj) {
        auto t = std::make_shared<T>(renderer);
        t->initialize();
        t->start();
        t->loadProperties(inObj);
        return t;
    }

    //ActorManagerの登録
    static void setActorManager(ActorManager* manager);
    ActorManager* getActorManager();

private:
    void initialize();
    void destroyTimer();

protected:
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<ComponentManager> mComponentManager;
    std::shared_ptr<Transform3D> mTransform;
    std::unique_ptr<Time> mDestroyTimer;
    std::string mTag;

private:
    State mState;

    static ActorManager* mActorManager;
};

