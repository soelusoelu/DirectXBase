#include "PlayerChickenConnection.h"
#include "../Component/ComponentManager.h"
#include "../Component/MeshComponent.h"
#include "../Component/PlayerMoveComponent.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"

PlayerChickenConnection::PlayerChickenConnection() :
    mPlayer(nullptr),
    mChicken(nullptr),
    mJumpTarget(nullptr),
    mPlayerPreviousPos(Vector3::zero),
    mChickenRadius(0.f) {
}

PlayerChickenConnection::~PlayerChickenConnection() = default;

void PlayerChickenConnection::initialize() {
    //メッシュ形状が変わらない・統一前提で
    if (mChicken) {
        auto mesh = mChicken->componentManager()->getComponent<MeshComponent>();
        if (mesh) {
            mChickenRadius = mesh->getRadius();
        }
    }
    mPlayerPreviousPos = mPlayer->owner()->transform()->getPosition();
}

void PlayerChickenConnection::connect() {
    if (mPlayer->isJumpStart()) {
        mChicken->transform()->rotate(Vector3::right * 90.f);
    }
    if (mPlayer->isJumpEnd()) {
        mChicken = mJumpTarget;
    }
    if (mPlayer->isWalking()) {
        auto pt = mPlayer->owner()->transform();
        auto ct = mChicken->transform();
        auto pos = pt->getPosition();
        pos.y = ct->getScale().y * mChickenRadius;
        pt->setPosition(pos);
        pos.y = 0;
        ct->setPosition(pos);
    }

    mPlayerPreviousPos = mPlayer->owner()->transform()->getPosition();
}

void PlayerChickenConnection::setPlayer(const GameObjectPtr player) {
    mPlayer = player->componentManager()->getComponent<PlayerMoveComponent>();
}

void PlayerChickenConnection::setChicken(const GameObjectPtr chicken) {
    mChicken = chicken;
}

std::shared_ptr<GameObject> PlayerChickenConnection::getChicken() const {
    return mChicken;
}

void PlayerChickenConnection::playerJumpTarget(const GameObjectPtr chicken) {
    if (mPlayer->isWalking()) {
        mJumpTarget = chicken;
        auto pos = mJumpTarget->transform()->getPosition();
        pos.y += mJumpTarget->transform()->getScale().y * mChickenRadius;
        mPlayer->setTargetPosition(pos);
    }
}
