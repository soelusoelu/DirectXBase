﻿#include "GameObject.h"
#include "GameObjectManager.h"
#include "Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Device/Time.h"

GameObject::GameObject(std::shared_ptr<Renderer> renderer) :
    mRenderer(renderer),
    mTransform(nullptr),
    mComponentManager(nullptr),
    mTag(""),
    mDestroyTimer(nullptr),
    mState(State::ACTIVE) {
}

GameObject::~GameObject() = default;

void GameObject::update() {
    mComponentManager->start();
    if (getActive()) {
        mComponentManager->update();

        computeWorldTransform();

        updateDestroyTimer();
    }
}

void GameObject::destroy() {
    mState = State::DEAD;
}

void GameObject::destroy(float sec) {
    if (mDestroyTimer) {
        return;
    }
    mDestroyTimer = std::make_unique<Time>(sec);
}

void GameObject::loadProperties(const rapidjson::Value& inObj) {
    mTransform->loadProperties(inObj);
}

void GameObject::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
}

void GameObject::setActive(bool value) {
    mState = (value) ? State::ACTIVE : State::NON_ACTIVE;

    mComponentManager->onSetActive(value);
}

bool GameObject::getActive() const {
    return mState == State::ACTIVE;
}

bool GameObject::isDead() const {
    return mState == State::DEAD;
}

void GameObject::setTag(const std::string& tag) {
    mTag = tag;
}

const std::string& GameObject::tag() const {
    return mTag;
}

std::shared_ptr<Renderer> GameObject::renderer() const {
    return mRenderer;
}

std::shared_ptr<Transform3D> GameObject::transform() const {
    return mTransform;
}

std::shared_ptr<ComponentManager> GameObject::componentManager() const {
    return mComponentManager;
}

void GameObject::setGameObjectManager(GameObjectManager* manager) {
    mGameObjectManager = manager;
}

GameObjectManager* GameObject::getGameObjectManager() {
    return mGameObjectManager;
}

std::shared_ptr<GameObject> GameObject::create(std::shared_ptr<Renderer> renderer) {
    auto obj = std::make_shared<GameObject>(renderer);
    obj->initialize();
    return obj;
}

void GameObject::computeWorldTransform() {
    if (mTransform->computeWorldTransform()) {
        mComponentManager->onUpdateWorldTransform();
    }
}

void GameObject::initialize() {
    if (mGameObjectManager) {
        mGameObjectManager->add(shared_from_this());
    }

    mTransform = std::make_shared<Transform3D>(shared_from_this());
    mComponentManager = std::make_shared<ComponentManager>(shared_from_this());
}

void GameObject::updateDestroyTimer() {
    if (!mDestroyTimer) {
        return;
    }
    mDestroyTimer->update();
    if (mDestroyTimer->isTime()) {
        mState = State::DEAD;
    }
}

GameObjectManager* GameObject::mGameObjectManager = nullptr;