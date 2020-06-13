#include "OneRemain.h"
#include "ComponentManager.h"
#include "PlayerChickenConnection.h"
#include "PlayerComponent.h"
#include "Sprite3D.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"

OneRemain::OneRemain() :
    Component(),
    mSprite(nullptr),
    mHiddenTimer(std::make_unique<Time>()),
    mOffset(Vector3::zero) {
}

OneRemain::~OneRemain() = default;

void OneRemain::start() {
    mSprite = owner()->componentManager()->getComponent<Sprite3D>();
}

void OneRemain::update() {
    if (!mSprite->getActive()) {
        return;
    }
    mHiddenTimer->update();
    if (mHiddenTimer->isTime()) {
        mHiddenTimer->reset();
        mSprite->setActive(false);
    }
}

void OneRemain::loadProperties(const rapidjson::Value& inObj) {
    float time;
    if (JsonHelper::getFloat(inObj, "hiddenTime", &time)) {
        mHiddenTimer->setLimitTime(time);
    }
    JsonHelper::getVector3(inObj, "offset", &mOffset);
}

void OneRemain::addObserver(const std::shared_ptr<PlayerComponent>& player, const std::shared_ptr<PlayerChickenConnection>& pcc) {
    player->onAwakenJump([&]() {
        mSprite->setActive(false);
        mHiddenTimer->reset();
    });
    pcc->onFailedCollection([&]() {
        mSprite->setActive(true);
        mHiddenTimer->reset();
    });
}

void OneRemain::setPositionAndRadius(const Vector3& pos, float radius) {
    mSprite->transform()->setPosition(pos + Vector3::back * radius + Vector3::up * radius + mOffset);
}
