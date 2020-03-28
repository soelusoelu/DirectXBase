#pragma once

#include "../Math/Math.h"
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
        NON_ACTIVE,
        DEAD
    };

public:
    Actor(std::shared_ptr<Renderer> renderer, const std::string& tag);
    virtual ~Actor();

    //すべての更新
    void update();

    //アクター削除
    void destroy();
    void destroy(float sec);

    //ゲッター、セッター
    std::shared_ptr<Renderer> renderer() const;
    std::shared_ptr<ComponentManager> componentManager() const;
    std::shared_ptr<Transform3D> transform() const;
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;
    const std::string& tag() const;

    //指定されたプロパティでアクターを生成
    static std::shared_ptr<Actor> create(std::shared_ptr<Renderer> renderer, const std::string& tag, const rapidjson::Value& inObj) {
        auto t = std::make_shared<Actor>(renderer, tag);
        t->initialize(inObj);
        return t;
    }

    //ActorManagerの登録
    static void setActorManager(ActorManager* manager);
    ActorManager* getActorManager();

protected:
    //ロード/セーブ
    virtual void loadProperties(const rapidjson::Value& inObj);
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const;

private:
    //初期化
    void initialize(const rapidjson::Value& inObj);
    //ワールド行列の更新
    void computeWorldTransform();
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

