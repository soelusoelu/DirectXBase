#include "PlayerComponent.h"
#include "ComponentManager.h"
#include "MeshComponent.h"
#include "PlayerJump.h"
#include "PlayerWalk.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"

PlayerComponent::PlayerComponent() :
    Component(),
    mWalk(nullptr),
    mJump(nullptr),
    mState(State::WALK),
    mIsWaitFirstFrame(false) {
}

PlayerComponent::~PlayerComponent() = default;

void PlayerComponent::start() {
    const auto& cm = owner()->componentManager();
    mWalk = cm->getComponent<PlayerWalk>();
    mJump = cm->getComponent<PlayerJump>();
    mJump->onJumpStart([&]() {mState = State::JUMP; });
    mJump->onJumpEnd([&]() { mState = State::WALK; });
    auto mesh = cm->getComponent<MeshComponent>();
    owner()->transform()->setPivot(Vector3::back * mesh->getRadius());
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
