#include "ChickenCollection.h"
#include "ComponentManager.h"
#include "FriedChickenComponent.h"
#include "FryState.h"
#include "IChickenFry.h"
#include "SoundComponent.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../Utility/LevelLoader.h"
#include <string>

ChickenCollection::ChickenCollection() :
    Component(),
    mSound(nullptr),
    mReplaySoundTimer(std::make_unique<Time>(1.f)),
    mCollectionKey(KeyCode::None),
    mCollectionPad(JoyCode::None) {
}

ChickenCollection::~ChickenCollection() = default;

void ChickenCollection::start() {
    mSound = owner()->componentManager()->getComponent<SoundComponent>();
}

void ChickenCollection::update() {
    if (mReplaySoundTimer->isTime()) {
        return;
    }
    mReplaySoundTimer->update();
}

void ChickenCollection::loadProperties(const rapidjson::Value& inObj) {
    std::string src;
    if (JsonHelper::getString(inObj, "collectionKey", &src)) {
        Keyboard::stringToKeyCode(src, &mCollectionKey);
    }
    if (JsonHelper::getString(inObj, "collectionPad", &src)) {
        JoyPad::stringToJoyCode(src, &mCollectionPad);
    }
}

bool ChickenCollection::tryCollection(const ChickenPtr& target) {
    if (!pressedCollectionKey()) {
        return false;
    }
    if (!successCollection(*target)) {
        setUpSound();
        return false;
    }

    //回収条件を満たした
    collection(target);

    return true;
}

bool ChickenCollection::pressedCollectionKey() const {
    bool isPressed = Input::joyPad()->getJoyDown(mCollectionPad);
#ifdef _DEBUG
    if (!isPressed) {
        isPressed = Input::keyboard()->getKeyDown(mCollectionKey);
    }
#endif // _DEBUG
    return isPressed;
}

bool ChickenCollection::successCollection(const FriedChickenComponent& target) const {
    //mFailedCollectionSubject->notify();

    //半分の面が良い以上で
    //return target.getFry().isBurntHalfSurfaces();
    //すべての面が普通以上で
    return target.getFry().isUpSelectState(FryState::USUALLY);
}

void ChickenCollection::setUpSound() {
    if (mReplaySoundTimer->isTime()) {
        mSound->playSE();
        mReplaySoundTimer->reset();
    }
}

void ChickenCollection::collection(const ChickenPtr& target) {
    target->setUp();
    target->setWaitingScore(true);
}
