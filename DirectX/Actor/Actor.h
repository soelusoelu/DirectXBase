#pragma once

#include "../Utility/Math.h"
#include <memory>

enum class ActorState {
    ACTIVE,
    DEAD
};

class ComponentManager;
class Transform2D;
class Time;
class ActorManager;

class Actor {
protected:
    Actor(const char* tag = "");
public:
    virtual ~Actor();

    //���ׂĂ̍X�V
    void update();
    //�A�N�^�[�ŗL�̍X�V
    virtual void updateActor() = 0;

    //���[���h�s��̍X�V
    void computeWorldTransform();

    //�A�N�^�[�폜
    static void destroy(Actor* actor);
    static void destroy(std::shared_ptr<Actor> actor);
    static void destroy(Actor* actor, float sec);
    static void destroy(std::shared_ptr<Actor> actor, float sec);

    //�Q�b�^�[�A�Z�b�^�[
    std::shared_ptr<ComponentManager> getComponentManager() const;
    std::shared_ptr<Transform2D> getTransform() const;
    ActorState getState() const;
    const char* getTag() const;

    //ActorManager�̓o�^
    static void setActorManager(ActorManager* manager);

private:
    void destroyTimer();

private:
    std::shared_ptr<ComponentManager> mComponentManager;
    std::shared_ptr<Transform2D> mTransform;
    std::unique_ptr<Time> mDestroyTimer;
    ActorState mState;
    const char* mTag;

    static ActorManager* mActorManager;
};

