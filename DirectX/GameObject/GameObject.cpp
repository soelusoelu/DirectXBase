#include "GameObject.h"
#include "GameObjectManager.h"
#include "Transform3D.h"
#include "../Component/ComponentManager.h"

GameObject::GameObject(const std::shared_ptr<Renderer>& renderer) :
    mRenderer(renderer),
    mTransform(nullptr),
    mComponentManager(nullptr),
    mTag(""),
    mIsActive(true) {
}

GameObject::~GameObject() {
    mComponentManager->finalize();
}

void GameObject::update() {
    mComponentManager->start();
    if (getActive()) {
        mComponentManager->update();
    }
    updateDestroyTimer();
}

void GameObject::lateUpdate() {
    if (getActive()) {
        mComponentManager->lateUpdate();

        computeWorldTransform();
    }
}

void GameObject::loadProperties(const rapidjson::Value& inObj) {
    mTransform->loadProperties(inObj);
}

void GameObject::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    mTransform->saveProperties(alloc, inObj);
}

void GameObject::setActive(bool value) {
    mIsActive = value;

    mComponentManager->onSetActive(value);
}

bool GameObject::getActive() const {
    return mIsActive;
}

void GameObject::setTag(const std::string& tag) {
    mTag = tag;
}

const std::string& GameObject::tag() const {
    return mTag;
}

const std::shared_ptr<Renderer>& GameObject::renderer() const {
    return mRenderer;
}

const std::shared_ptr<Transform3D>& GameObject::transform() const {
    return mTransform;
}

const std::shared_ptr<ComponentManager>& GameObject::componentManager() const {
    return mComponentManager;
}

void GameObject::setGameObjectManager(GameObjectManager* manager) {
    mGameObjectManager = manager;
}

GameObjectManager* GameObject::getGameObjectManager() {
    return mGameObjectManager;
}

std::shared_ptr<GameObject> GameObject::create(const std::shared_ptr<Renderer>& renderer) {
    auto obj = std::make_shared<GameObject>(renderer);
    obj->initialize();
    return obj;
}

void GameObject::initialize() {
    if (mGameObjectManager) {
        mGameObjectManager->add(shared_from_this());
    }

    mTransform = std::make_shared<Transform3D>(shared_from_this());
    mComponentManager = std::make_shared<ComponentManager>();
}

void GameObject::computeWorldTransform() {
    if (mTransform->computeWorldTransform()) {
        mComponentManager->onUpdateWorldTransform();
    }
}

GameObjectManager* GameObject::mGameObjectManager = nullptr;
