#include "PlayerChickenConnection.h"
#include "ComponentManager.h"
#include "FriedChickenComponent.h"
#include "MeshComponent.h"
#include "PlayerMoveComponent.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Input/Input.h"
#include "../Input/JoyPad.h"
#include "../Input/Keyboard.h"

PlayerChickenConnection::PlayerChickenConnection(std::shared_ptr<GameObject> owner) :
    Component(owner, "PlayerChickenConnection", 150),
    mPlayer(nullptr),
    mChicken(nullptr),
    mJumpTarget(nullptr),
    mPlayerPreviousPos(Vector3::zero),
    mChickenRadius(0.f) {
}

PlayerChickenConnection::~PlayerChickenConnection() = default;

void PlayerChickenConnection::start() {
    //メッシュ形状が変わらない・統一前提で
    if (mChicken) {
        auto mesh = mChicken->componentManager()->getComponent<MeshComponent>();
        if (mesh) {
            mChickenRadius = mesh->getRadius();
        }
    }
    mPlayerPreviousPos = mPlayer->owner()->transform()->getPosition();
}

void PlayerChickenConnection::update() {
    if (mPlayer->isJumpStart()) {
        auto chickenComp = mChicken->componentManager()->getComponent<FriedChickenComponent>();
        chickenComp->changeSurface();
        mChicken->transform()->rotate(Vector3::right * 180.f);
    }
    if (mPlayer->isJumpEnd()) {
        mChicken = mJumpTarget;
    }
    if (mPlayer->isWalking()) {
        setPlayerPosOnTheChicken(*mChicken);
        setChickenPosUnderThePlayer();

        collection();
    }

    mPlayerPreviousPos = mPlayer->owner()->transform()->getPosition();
}

void PlayerChickenConnection::setPlayer(const GameObjectPtr& player) {
    mPlayer = player->componentManager()->getComponent<PlayerMoveComponent>();
}

void PlayerChickenConnection::setChicken(const GameObjectPtr& chicken) {
    mChicken = chicken;
}

std::shared_ptr<GameObject> PlayerChickenConnection::getChicken() const {
    return mChicken;
}

void PlayerChickenConnection::playerJumpTarget(const GameObjectPtr& chicken) {
    if (!chicken) {
        return;
    }

    if (mPlayer->isWalking()) {
        mJumpTarget = chicken;
        auto pos = mJumpTarget->transform()->getPosition();
        pos.y += mJumpTarget->transform()->getScale().y * mChickenRadius;
        mPlayer->setTargetPosition(pos);
    }
}

void PlayerChickenConnection::setPlayerPosOnTheChicken(const GameObject& chicken) {
    auto pt = mPlayer->owner()->transform();
    auto pos = pt->getPosition();
    pos.y = chicken.transform()->getScale().y * mChickenRadius;
    mPlayer->owner()->transform()->setPosition(pos);
}

void PlayerChickenConnection::setChickenPosUnderThePlayer() {
    auto pos = mPlayer->owner()->transform()->getPosition();
    pos.y = 0;
    mChicken->transform()->setPosition(pos);
}

void PlayerChickenConnection::collection() {
    if (Input::joyPad()->getJoyDown(JoyCode::X) || Input::keyboard()->getKeyDown(KeyCode::E)) {
        if (!mJumpTarget) {
            return;
        }

        auto chickenComp = mChicken->componentManager()->getComponent<FriedChickenComponent>();
        chickenComp->finishFryed();

        auto pos = mJumpTarget->transform()->getPosition();
        mPlayer->owner()->transform()->setPosition(pos);
        setPlayerPosOnTheChicken(*mJumpTarget);
        mChicken = mJumpTarget;
    }
}
