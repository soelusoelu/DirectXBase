#include "PlayerChickenConnection.h"
#include "ChickenCollection.h"
#include "ComponentManager.h"
#include "FriedChickenComponent.h"
#include "IPlayerJump.h"
#include "IPlayerWalk.h"
#include "MeshComponent.h"
#include "PlayerComponent.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"
#include <string>

PlayerChickenConnection::PlayerChickenConnection() :
    Component(),
    mPlayer(nullptr),
    mChicken(nullptr),
    mJumpTarget(nullptr),
    mChickenRadius(0.f),
    mCollection(nullptr),
    mIsJumpRoll(true) {
}

PlayerChickenConnection::~PlayerChickenConnection() = default;

void PlayerChickenConnection::awake() {
    auto coll = GameObjectCreater::create("ChickenCollection");
    mCollection = coll->componentManager()->getComponent<ChickenCollection>();
}

void PlayerChickenConnection::start() {
    //メッシュ形状が変わらない・統一前提で
    if (mChicken) {
        auto mesh = mChicken->owner()->componentManager()->getComponent<MeshComponent>();
        if (mesh) {
            mChickenRadius = mesh->getRadius();
        }
    }

    mPlayer->getJumpState()->onJumpStart([&]() {
        if (mIsJumpRoll) {
            mChicken->owner()->transform()->rotate(Vector3::right * 180.f);
        }
    });
    mPlayer->getJumpState()->onJumpEnd([&]() {
        mChicken = mJumpTarget;
    });
}

void PlayerChickenConnection::update() {
    auto jump = mPlayer->getJumpState();
    if (jump->isJumping()) {
        trackingJumpTarget();
    }
    if (mPlayer->isWalking()) {
        jump->canJump(existsJumpTarget());
        setPlayerPosOnTheChicken(*mChicken);
        setChickenPosUnderThePlayer();

        rollChicken();
        collection();
        tooBurntUnderThePlayer();
        eatenChicken();
    }
}

void PlayerChickenConnection::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getBool(inObj, "isJumpRoll", &mIsJumpRoll);
}

void PlayerChickenConnection::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("IsJumpRoll", mIsJumpRoll);
}

void PlayerChickenConnection::setPlayer(const std::shared_ptr<PlayerComponent>& player) {
    mPlayer = player;
}

void PlayerChickenConnection::setChicken(const ChickenPtr & chicken) {
    mChicken = chicken;
}

const std::shared_ptr<FriedChickenComponent>& PlayerChickenConnection::getChicken() const {
    return mChicken;
}

bool PlayerChickenConnection::existsJumpTarget() const {
    return (mJumpTarget) ? true : false;
}

Vector3 PlayerChickenConnection::getJumpTargetTopPos() const {
    if (!mJumpTarget) {
        return Vector3::zero;
    }
    auto trans = mJumpTarget->owner()->transform();
    auto pos = trans->getPosition();
    auto offsetY = mChickenRadius * trans->getScale().y;
    pos.y += offsetY;
    return pos;
}

void PlayerChickenConnection::setJumpTargetIfWalking(const ChickenPtr & chicken) {
    if (mPlayer->isWalking()) {
        mJumpTarget = chicken;
    }
}

void PlayerChickenConnection::onFailedCollection(const std::function<void()>& f) {
    mCollection->onFailedCollection(f);
}

float PlayerChickenConnection::getScalingRadiusFromChicken() const {
    return mChickenRadius * mChicken->owner()->transform()->getScale().x;
}

void PlayerChickenConnection::setPlayerPosOnTheChicken(const FriedChickenComponent & chicken) {
    auto pt = mPlayer->owner()->transform();
    auto pos = pt->getPosition();
    pos.y = chicken.owner()->transform()->getScale().y * mChickenRadius;
    mPlayer->owner()->transform()->setPosition(pos);
}

void PlayerChickenConnection::setChickenPosUnderThePlayer() {
    if (mChicken->isTooBurnt()) {
        return;
    }
    auto pos = mPlayer->owner()->transform()->getPosition();
    pos.y = 0;
    mChicken->owner()->transform()->setPosition(pos);
}

void PlayerChickenConnection::trackingJumpTarget() {
    if (!mJumpTarget) {
        return;
    }

    auto pos = mJumpTarget->owner()->transform()->getPosition();
    pos.y += mJumpTarget->owner()->transform()->getScale().y * mChickenRadius;
    mPlayer->getJumpState()->setTargetPosition(pos);
}

void PlayerChickenConnection::rollChicken() {
    const auto& dir = mPlayer->getWalkState()->getMoveDirection();
    if (Math::nearZero(dir.x) && Math::nearZero(dir.z)) {
        return;
    }
    mChicken->roll(dir);
}

void PlayerChickenConnection::collection() {
    if (!mJumpTarget) {
        return;
    }
    bool result = mCollection->tryCollection(mChicken);
    if (result) {
        setPlayerPosOnTheJumpTarget();
    }
}

void PlayerChickenConnection::setPlayerPosOnTheJumpTarget() {
    auto pos = mJumpTarget->owner()->transform()->getPosition();
    mPlayer->owner()->transform()->setPosition(pos);
    setPlayerPosOnTheChicken(*mJumpTarget);
    mChicken = mJumpTarget;
}

void PlayerChickenConnection::tooBurntUnderThePlayer() {
    if (!mChicken) {
        return;
    }
    if (!mJumpTarget) {
        return;
    }
    if (!mChicken->isTooBurnt()) {
        return;
    }
    setPlayerPosOnTheJumpTarget();
}

void PlayerChickenConnection::eatenChicken() {
    if (!mChicken) {
        return;
    }
    if (!mChicken->isEaten()) {
        return;
    }
    if (!mJumpTarget) {
        return;
    }

    setPlayerPosOnTheJumpTarget();
}
