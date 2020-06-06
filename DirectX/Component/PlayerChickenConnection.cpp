#include "PlayerChickenConnection.h"
#include "ComponentManager.h"
#include "FriedChickenComponent.h"
#include "IChickenFry.h"
#include "IPlayerJump.h"
#include "IPlayerWalk.h"
#include "MeshComponent.h"
#include "PlayerComponent.h"
#include "SoundComponent.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"

PlayerChickenConnection::PlayerChickenConnection() :
    Component(),
    mPlayer(nullptr),
    mChicken(nullptr),
    mJumpTarget(nullptr),
    mChickenRadius(0.f),
    mSound(nullptr),
    mCollectionKey(KeyCode::None),
    mCollectionPad(JoyCode::None),
    mIsJumpRoll(true) {
}

PlayerChickenConnection::~PlayerChickenConnection() = default;

void PlayerChickenConnection::start() {
    //メッシュ形状が変わらない・統一前提で
    if (mChicken) {
        auto mesh = mChicken->owner()->componentManager()->getComponent<MeshComponent>();
        if (mesh) {
            mChickenRadius = mesh->getRadius();
        }
    }
    mSound = owner()->componentManager()->getComponent<SoundComponent>();
}

void PlayerChickenConnection::update() {
    auto jump = mPlayer->getJumpState();
    if (jump->isJumpStart()) {
        if (mIsJumpRoll) {
            mChicken->owner()->transform()->rotate(Vector3::right * 180.f);
        }
    }
    if (jump->isJumping()) {
        trackingJumpTarget();
    }
    if (jump->isJumpEnd()) {
        mChicken = mJumpTarget;
    }
    if (mPlayer->isWalking()) {
        bool res = (mJumpTarget) ? true : false;
        jump->canJump(res);
        setPlayerPosOnTheChicken(*mChicken);
        setChickenPosUnderThePlayer();

        rollChicken();
        collection();
        tooBurntUnderThePlayer();
        eatenChicken();
    }
}

void PlayerChickenConnection::loadProperties(const rapidjson::Value & inObj) {
    std::string src;
    if (JsonHelper::getString(inObj, "collectionKey", &src)) {
        Keyboard::stringToKeyCode(src, &mCollectionKey);
    }
    if (JsonHelper::getString(inObj, "collectionPad", &src)) {
        JoyPad::stringToJoyCode(src, &mCollectionPad);
    }
    JsonHelper::getBool(inObj, "isJumpRoll", &mIsJumpRoll);
}

void PlayerChickenConnection::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("IsJumpRoll", mIsJumpRoll);
}

void PlayerChickenConnection::setPlayer(const GameObject & player) {
    mPlayer = player.componentManager()->getComponent<PlayerComponent>();
}

void PlayerChickenConnection::setChicken(const ChickenPtr & chicken) {
    mChicken = chicken;
}

const std::shared_ptr<FriedChickenComponent>& PlayerChickenConnection::getChicken() const {
    return mChicken;
}

bool PlayerChickenConnection::isJumpTarget() const {
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

void PlayerChickenConnection::setPlayerJumpTarget(const ChickenPtr & chicken) {
    if (mPlayer->isWalking()) {
        mJumpTarget = chicken;
    }
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

void PlayerChickenConnection::setPlayerPosOnTheJumpTarget() {
    auto pos = mJumpTarget->owner()->transform()->getPosition();
    mPlayer->owner()->transform()->setPosition(pos);
    setPlayerPosOnTheChicken(*mJumpTarget);
    mChicken = mJumpTarget;
}

void PlayerChickenConnection::collection() {
    bool isPressed = Input::joyPad()->getJoyDown(mCollectionPad);
#ifdef _DEBUG
    if (!isPressed) {
        isPressed = Input::keyboard()->getKeyDown(mCollectionKey);
    }
#endif // _DEBUG
    if (!isPressed) {
        return;
    }
    if (!mJumpTarget) {
        return;
    }
    //半分の面が良い以上で
    //if (!mChicken->getFry().isBurntHalfSurfaces()) {
    //    return;
    //}
    //すべての面が普通以上で
    if (!mChicken->getFry().isUpSelectState(FryState::USUALLY)) {
        mSound->playSE();
        return;
    }

    mChicken->setUp();
    mChicken->setWaitingScore(true);

    setPlayerPosOnTheJumpTarget();
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

    mChicken->finishFryed();

    setPlayerPosOnTheJumpTarget();
}
