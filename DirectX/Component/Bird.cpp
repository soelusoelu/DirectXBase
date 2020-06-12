#include "Bird.h"
#include "BirdOrbit.h"
#include "ComponentManager.h"
#include "FriedChickenComponent.h"
#include "MeshComponent.h"
#include "SoundComponent.h"
#include "SphereCollisionComponent.h"
#include "../Device/Random.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Device/Time.h"
#include "../Utility/LevelLoader.h"
#include <vector>

Bird::Bird() :
    Component(),
    mOrbit(nullptr),
    mRestartTimer(std::make_unique<Time>()),
    mMesh(nullptr),
    mCollider(nullptr),
    mSound(nullptr),
    mTarget(nullptr),
    mState(State::WAIT),
    mMoveSpeed(0.f),
    mClimbSpeed(0.f),
    mCollideRadius(0.f) {
}

Bird::~Bird() = default;

void Bird::start() {
    auto compMana = owner()->componentManager();
    mOrbit = compMana->getComponent<BirdOrbit>();
    mMesh = compMana->getComponent<MeshComponent>();
    mMesh->setActive(false);
    mCollider = compMana->getComponent<SphereCollisionComponent>();
    mSound = compMana->getComponent<SoundComponent>();
}

void Bird::update() {
    if (mState == State::WAIT) {
        waiting();
    } else if (mState == State::PREDICT_LINE) {
        predictLine();
    } else if (mState == State::MOVE) {
        move();
        updateCollider();
        takeChicken();
        isEndMoving();
    } else if (mState == State::HIT_MOVE) {
        hitMove();
        isEndMoving();
    }
}

void Bird::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getFloat(inObj, "moveSpeed", &mMoveSpeed);
    JsonHelper::getFloat(inObj, "climbSpeed", &mClimbSpeed);
    JsonHelper::getFloat(inObj, "collideRadius", &mCollideRadius);
    float time;
    if (JsonHelper::getFloat(inObj, "restartTimer", &time)) {
        mRestartTimer->setLimitTime(time);
    }
}

void Bird::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    ComponentDebug::DebugInfo info;
    info.first = "State";
    if (mState == State::WAIT) {
        info.second = "WAIT";
    } else if (mState == State::PREDICT_LINE) {
        info.second = "PREDICT_LINE";
    } else if (mState == State::MOVE) {
        info.second = "MOVE";
    } else if (mState == State::HIT_MOVE) {
        info.second = "HIT_MOVE";
    }
    inspect->emplace_back(info);
}

void Bird::waiting() {
    mRestartTimer->update();
    auto temp = mRestartTimer->countDownTime();
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

void Bird::updateCollider() {
    mCollider->set(owner()->transform()->getPosition() + Vector3::forward, mCollideRadius);
}

void Bird::hitMove() {
    auto amount = Vector3::zero;
    amount.x = -mMoveSpeed * Time::deltaTime;
    amount.y = mClimbSpeed * Time::deltaTime;;
    owner()->transform()->translate(amount);
}

void Bird::takeChicken() {
    if (!mTarget) {
        return;
    }

    std::shared_ptr<GameObject> hit = nullptr;
    for (const auto& col : mCollider->onCollisionEnter()) {
        if (col->owner()->tag() != "FriedChicken") {
            continue;
        }
        hit = col->owner();
        break;
    }

    if (hit) {
        hit->componentManager()->getComponent<FriedChickenComponent>()->eaten();
        mState = State::HIT_MOVE;
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
    auto chickens = owner()->getGameObjectManager()->findGameObjects("FriedChicken");
    std::vector<std::shared_ptr<GameObject>> result;
    //アクティブな唐揚げのうち、揚げている途中のものだけ取り出す
    for (const auto& chicken : chickens) {
        auto comp = chicken->componentManager()->getComponent<FriedChickenComponent>();
        if (comp->isFrying()) {
            result.emplace_back(chicken);
        }
    }
    //揚げてる最中の唐揚げがなければ終わり
    if (result.empty()) {
        mRestartTimer->reset();
        mState = State::WAIT;
        return;
    }
    auto randomIndex = Random::randomRange(0, result.size());
    mTarget = result[randomIndex];

    auto posZ = mTarget->transform()->getPosition().z;
    owner()->transform()->setPosition(Vector3(15.f, 0.f, posZ));
    mMesh->setActive(true);
    mOrbit->setActive(true);
    mOrbit->setPositionZ(posZ);
    mSound->playSE();
}

void Bird::finalize() {
    mMesh->setActive(false);
    mOrbit->setActive(false);
}
