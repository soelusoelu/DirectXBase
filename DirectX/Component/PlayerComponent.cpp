#include "PlayerComponent.h"
#include "ComponentManager.h"
#include "PlayerJump.h"
#include "PlayerWalk.h"
#include "../GameObject/GameObject.h"

PlayerComponent::PlayerComponent() :
    Component(),
    mWalk(nullptr),
    mJump(nullptr),
    mState(State::WALK),
    mIsWaitFirstFrame(false) {
}

PlayerComponent::~PlayerComponent() = default;

void PlayerComponent::start() {
    mWalk = owner()->componentManager()->getComponent<PlayerWalk>();
    mJump = owner()->componentManager()->getComponent<PlayerJump>();
    mJump->onJumpStart([&]() {mState = State::JUMP; });
    mJump->addJumpEndObserver([&]() { mState = State::WALK; });
}

void PlayerComponent::update() {
    if (!mIsWaitFirstFrame) {
        mIsWaitFirstFrame = true;
        return;
    }

    if (mState == State::WALK) {
        mWalk->walkUpdate();
        mJump->jump();
    } else if (mState == State::JUMP) {
        mJump->jumpUpdate();
    }
}

std::shared_ptr<IPlayerWalk> PlayerComponent::getWalkState() const {
    return mWalk;
}

std::shared_ptr<IPlayerJump> PlayerComponent::getJumpState() const {
    return mJump;
}

bool PlayerComponent::isWalking() const {
    return mState == State::WALK;
}
