#pragma once

#include <memory>
#include <list>

class Actor;
class PlayerActor;

class ActorManager {
    using ActorPtr = std::shared_ptr<Actor>;
    using ActorPtrList = std::list<ActorPtr>;

public:
    ActorManager();
    ~ActorManager();
    void update();
    void add(ActorPtr add);
    void clear();
    //アクター配列の中からプレイヤーを取得
    std::shared_ptr<PlayerActor> getPlayer() const;

    //一番最初に見つかったActorの取得
    template<typename T>
    std::shared_ptr<T> getActor() const {
        std::shared_ptr<T> actor = nullptr;
        for (const auto& a : mActors) {
            actor = std::dynamic_pointer_cast<T>(a);
            if (actor) {
                return actor;
            }
        }
        for (const auto& a : mPendingActors) {
            actor = std::dynamic_pointer_cast<T>(a);
            if (actor) {
                return actor;
            }
        }
        //最後まで見つからなければnullptrを返す
        return actor;
    }

private:
    void remove();
    void movePendingToMain();

    ActorManager(const ActorManager&) = delete;
    ActorManager& operator=(const ActorManager&) = delete;

private:
    ActorPtrList mActors;
    ActorPtrList mPendingActors;
    bool mUpdatingActors;
};

