﻿#include "Actor.h"
#include "ActorManager.h"
#include "Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Device/Time.h"

Actor::Actor(std::shared_ptr<Renderer> renderer, const char* tag) :
    mRenderer(renderer),
    mComponentManager(std::make_shared<ComponentManager>()),
    mTransform(std::make_shared<Transform3D>(this)),
    mDestroyTimer(nullptr),
    mState(ActorState::ACTIVE),
    mTag(tag) {
    if (mActorManager) {
        mActorManager->add(this);
    }
}

Actor::~Actor() = default;

void Actor::update() {
    mComponentManager->start();
    if (mState == ActorState::ACTIVE) {
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
    mState = ActorState::DEAD;
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

ActorState Actor::getState() const {
    return mState;
}

const char* Actor::tag() const {
    return mTag;
}

void Actor::setActorManager(ActorManager* manager) {
    mActorManager = manager;
}

ActorManager* Actor::getActorManager() {
    return mActorManager;
}

void Actor::destroyTimer() {
    if (!mDestroyTimer) {
        return;
    }
    mDestroyTimer->update();
    if (mDestroyTimer->isTime()) {
        mState = ActorState::DEAD;
    }
}

ActorManager* Actor::mActorManager = nullptr;
