#include "PlayerComponent.h"
#include "ComponentManager.h"
#include "PlayerJump.h"
#include "PlayerWalk.h"
#include "../GameObject/GameObject.h"

PlayerComponent::PlayerComponent() :
    Component(10),
    mWalk(nullptr),
    mJump(nullptr),
    mState(State::WALK),
    mIsWaitFirstFrame(false) {
}

PlayerComponent::~PlayerComponent() = default;

void PlayerComponent::start() {
    mWalk = owner()->componentManager()->getComponent<PlayerWalk>();
    mJump = owner()->componentManager()->getComponent<PlayerJump>();
}

void PlayerComponent::update() {
    if (!mIsWaitFirstFrame) {
        mIsWaitFirstFrame = true;
        return;
    }

    if (mState == State::WALK) {
        mWalk->walkUpdate();
        mJump->jump();

        if (mJump->isJumpStart()) {
            mState = State::JUMP;
        }
    } else if (mState == State::JUMP) {
        mJump->jumpUpdate();

        if (mJump->isJumpEnd()) {
            mState = State::WALK;
        }
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
