#pragma once

#include <memory>
#include <list>

class Actor;
class PlayerActor;

class ActorManager {
public:
    ActorManager();
    ~ActorManager();
    void update();
    void add(Actor* add);
    void clear();

    //�A�N�^�[�z��̒�����v���C���[���擾
    std::shared_ptr<PlayerActor> getPlayer() const;
    //��ԍŏ��Ɍ�������Actor�̎擾
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
        //�Ō�܂Ō�����Ȃ����nullptr��Ԃ�
        return actor;
    }

private:
    void remove();
    void movePendingToMain();

    ActorManager(const ActorManager&) = delete;
    ActorManager& operator=(const ActorManager&) = delete;

private:
    std::list<std::shared_ptr<Actor>> mActors;
    std::list<std::shared_ptr<Actor>> mPendingActors;
    bool mUpdatingActors;
};

