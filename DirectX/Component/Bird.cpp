﻿#include "Bird.h"
#include "BirdOrbit.h"
#include "ComponentManager.h"
#include "FriedChickenComponent.h"
#include "MeshComponent.h"
#include "SoundComponent.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Device/Time.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

Bird::Bird(std::shared_ptr<GameObject> owner) :
    Component(owner, "Bird"),
    mOrbit(nullptr),
    mRestartTimer(std::make_unique<Time>()),
    mMesh(nullptr),
    mSound(nullptr),
    mTarget(nullptr),
    mState(State::WAIT),
    mMoveSpeed(0.f) {
}

Bird::~Bird() = default;

void Bird::start() {
    auto compMana = owner()->componentManager();
    mOrbit = compMana->getComponent<BirdOrbit>();
    mMesh = compMana->getComponent<MeshComponent>();
    mMesh->setActive(false);
    mSound = compMana->getComponent<SoundComponent>();
    auto t = owner()->transform();
    t->rotate(Vector3::up, 90.f);
    t->rotate(Vector3::forward, 90.f);
    t->setPivot(Vector3::up * mMesh->getRadius());
}

void Bird::update() {
    if (mState == State::WAIT) {
        waiting();
    } else if (mState == State::PREDICT_LINE) {
        predictLine();
    } else if (mState == State::MOVE) {
        move();
        takeChicken();
        isEndMoving();
    }
}

void Bird::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "moveSpeed", &mMoveSpeed);
    float time;
    if (JsonHelper::getFloat(inObj, "restartTimer", &time)) {
        mRestartTimer->setLimitTime(time);
    }
}

void Bird::drawDebugInfo(DebugInfoList * inspect) const {
    DebugInfo info;
    info.first = "State";
    if (mState == State::WAIT) {
        info.second = "WAIT";
    } else if (mState == State::PREDICT_LINE) {
        info.second = "PREDICT_LINE";
    } else if (mState == State::MOVE) {
        info.second = "MOVE";
    }
    inspect->emplace_back(info);
    info.first = "MoveSpeed";
    info.second = StringUtil::floatToString(mMoveSpeed);
    inspect->emplace_back(info);
    info.first = "RestartTimer";
    info.second = StringUtil::floatToString(mRestartTimer->limitTime());
    inspect->emplace_back(info);
}

void Bird::waiting() {
    mRestartTimer->update();
    auto temp = mRestartTimer->limitTime() - mRestartTimer->currentTime();
    if (temp <= mOrbit->getTime()) {
        mState = State::PREDICT_LINE;
        initialize();
    }
}

void Bird::predictLine() {
    mRestartTimer->update();
    if (mRestartTimer->isTime()) {
        mRestartTimer->reset();
        mState = State::MOVE;
    }
}

void Bird::move() {
    if (!mTarget) {
        mState = State::WAIT;
        finalize();
        return;
    }
    owner()->transform()->translate(Vector3::left * Time::deltaTime * mMoveSpeed);
}

void Bird::takeChicken() {
    if (!mTarget) {
        return;
    }
    auto birdPos = owner()->transform()->getPosition();
    auto targetPos = mTarget->transform()->getPosition();
    //鳥が唐揚げの右側にいるときのみ
    if (birdPos.x < targetPos.x) {
        return;
    }
    //鳥と唐揚げの距離が近いときのみ
    if (Math::abs(birdPos.x - targetPos.x) > 1.f) {
        return;
    }
    if (Math::abs(birdPos.z - targetPos.z) > 1.f) {
        return;
    }

    auto fc = mTarget->componentManager()->getComponent<FriedChickenComponent>();
    if (fc) {
        fc->eaten();
    }
}

void Bird::isEndMoving() {
    auto posX = owner()->transform()->getPosition().x;
    if (posX < 0.f && !mMesh->isVisible()) {
        mState = State::WAIT;
        finalize();
    }
}

void Bird::initialize() {
    mTarget = owner()->getGameObjectManager()->randomFind("FriedChicken");
    auto posZ = mTarget->transform()->getPosition().z;
    owner()->transform()->setPosition(Vector3(15.f, 0.f, posZ));
    mMesh->setActive(true);
    mOrbit->setActive(true);
    mOrbit->setPositionZ(posZ);
    mSound->play();
}

void Bird::finalize() {
    mMesh->setActive(false);
    mOrbit->setActive(false);
}
