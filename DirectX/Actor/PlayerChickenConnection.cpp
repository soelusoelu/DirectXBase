#include "PlayerChickenConnection.h"
#include "FriedChicken.h"
#include "PlayerActor.h"
#include "Transform3D.h"

PlayerChickenConnection::PlayerChickenConnection() = default;

PlayerChickenConnection::~PlayerChickenConnection() = default;

void PlayerChickenConnection::connect() {
    if (mPlayer->isJump()) {
        if (mChicken != mJumpTarget) {
            mChicken = mJumpTarget;
        }
        return;
    }
    mChicken->transform()->setPosition(mPlayer->transform()->getPosition());
}

void PlayerChickenConnection::setPlayer(const std::shared_ptr<PlayerActor> player) {
    mPlayer = player;
}

void PlayerChickenConnection::setChicken(const std::shared_ptr<FriedChicken> chicken) {
    mChicken = chicken;
}

std::shared_ptr<FriedChicken> PlayerChickenConnection::getChicken() const {
    return mChicken;
}

void PlayerChickenConnection::playerJumpTarget(const std::shared_ptr<FriedChicken> chicken) {
    mJumpTarget = chicken;
    mPlayer->setTargetPosition(mJumpTarget->transform()->getPosition());
}
