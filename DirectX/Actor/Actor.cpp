#include "Actor.h"
#include "ActorManager.h"
#include "Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Device/Time.h"
#include "../Utility/LevelLoader.h"

Actor::Actor(std::shared_ptr<Renderer> renderer, const std::string& tag) :
    mRenderer(renderer),
    mComponentManager(std::make_shared<ComponentManager>()),
    mTransform(std::make_shared<Transform3D>(this)),
    mDestroyTimer(nullptr),
    mTag(tag),
    mState(State::ACTIVE) {
}

Actor::~Actor() = default;

void Actor::update() {
    mComponentManager->start();
    if (mState == State::ACTIVE) {
        computeWorldTransform();

        mComponentManager->update();
        updateActor();

        computeWorldTransform();

        destroyTimer();
    }
}

void Actor::computeWorldTransform() {
    if (mTransform->computeWorldTransform()) {
        mComponentManager->onUpdateWorldTransform();
    }
}

void Actor::destroy() {
    mState = State::DEAD;
}

void Actor::destroy(float sec) {
    if (mDestroyTimer) {
        return;
    }
    mDestroyTimer = std::make_unique<Time>(sec);
}

std::shared_ptr<Renderer> Actor::renderer() const {
    return mRenderer;
}

std::shared_ptr<ComponentManager> Actor::componentManager() const {
    return mComponentManager;
}

std::shared_ptr<Transform3D> Actor::transform() const {
    return mTransform;
}

bool Actor::isDead() const {
    return mState == State::DEAD;
}

const std::string& Actor::tag() const {
    return mTag;
}

void Actor::loadProperties(const rapidjson::Value& inObj) {
    mTransform->loadProperties(inObj);
}

void Actor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    mTransform->saveProperties(alloc, inObj);
}

void Actor::setActorManager(ActorManager * manager) {
    mActorManager = manager;
}

ActorManager* Actor::getActorManager() {
    return mActorManager;
}

void Actor::addToManager() {
    if (mActorManager) {
        mActorManager->add(shared_from_this());
    }
}

void Actor::destroyTimer() {
    if (!mDestroyTimer) {
        return;
    }
    mDestroyTimer->update();
    if (mDestroyTimer->isTime()) {
        mState = State::DEAD;
    }
}

ActorManager* Actor::mActorManager = nullptr;
