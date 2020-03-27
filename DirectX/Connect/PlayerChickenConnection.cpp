﻿#include "PlayerChickenConnection.h"
#include "../Actor/FriedChicken.h"
#include "../Actor/PlayerActor.h"
#include "../Actor/Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Component/MeshComponent.h"

PlayerChickenConnection::PlayerChickenConnection() :
    mPlayer(nullptr),
    mChicken(nullptr),
    mJumpTarget(nullptr),
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
}

void PlayerChickenConnection::connect() {
    if (mPlayer->isJump()) {
        if (mChicken != mJumpTarget) {
            mChicken = mJumpTarget;
        }
        return;
    }
    auto pt = mPlayer->transform();
    auto ct = mChicken->transform();
    auto pos = pt->getPosition();
    pos.y = ct->getScale().y * mChickenRadius;
    pt->setPosition(pos);
    pos.y = 0;
    ct->setPosition(pos);
}

void PlayerChickenConnection::setPlayer(const PlayerPtr player) {
    mPlayer = player;
}

void PlayerChickenConnection::setChicken(const ChickenPtr chicken) {
    mChicken = chicken;
}

std::shared_ptr<FriedChicken> PlayerChickenConnection::getChicken() const {
    return mChicken;
}

void PlayerChickenConnection::playerJumpTarget(const ChickenPtr chicken) {
    mJumpTarget = chicken;
    auto pos = mJumpTarget->transform()->getPosition();
    pos.y += mJumpTarget->transform()->getScale().y * mChickenRadius;
    mPlayer->setTargetPosition(pos);
}