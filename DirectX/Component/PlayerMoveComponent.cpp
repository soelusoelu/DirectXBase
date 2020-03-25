#include "PlayerMoveComponent.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Component/ComponentManager.h"
#include "../Component/MeshComponent.h"
#include "../Device/Time.h"
#include "../Input/Input.h"
#include "../Input/Keyboard.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

PlayerMoveComponent::PlayerMoveComponent(std::shared_ptr<Actor> owner) :
    Component(owner, "PlayerMoveComponent", 10),
    mMesh(nullptr),
    mState(State::WALK),
    mJumpTargetPosition(Vector3::zero),
    mMoveSpeed(1.f) {
}

PlayerMoveComponent::~PlayerMoveComponent() = default;

void PlayerMoveComponent::start() {
    mMesh = owner()->componentManager()->getComponent<MeshComponent>();
    if (mMesh) {
        owner()->transform()->setPivot(Vector3::down * mMesh->getRadius());
    }
}

void PlayerMoveComponent::update() {
    jump();
    move();
    jumpMove();
    jumpStop();
}

void PlayerMoveComponent::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "moveSpeed", &mMoveSpeed);
}

void PlayerMoveComponent::drawDebugInfo(debugInfoList * inspect) const {
    debugInfo info;
    info.first = "State";
    if (mState == State::WALK) {
        info.second = "WALK";
    } else if (mState == State::JUMP) {
        info.second = "JUMP";
    }
    inspect->emplace_back(info);
    info.first = "MoveSpeed";
    info.second = StringUtil::floatToString(mMoveSpeed);
    inspect->emplace_back(info);
}

bool PlayerMoveComponent::isJump() const {
    return mState == State::JUMP;
}

void PlayerMoveComponent::setTargetPosition(const Vector3 & pos) {
    if (mState == State::JUMP) {
        return;
    }
    mJumpTargetPosition = pos;
}

void PlayerMoveComponent::jump() {
    if (mState != State::WALK) {
        return;
    }
    if (Input::keyboard()->getKeyDown(KeyCode::Space)) {
        mState = State::JUMP;
    }
}

void PlayerMoveComponent::move() {
    if (mState != State::WALK) {
        return;
    }
    auto h = Input::keyboard()->horizontal();
    auto v = Input::keyboard()->vertical();
    if (!Math::nearZero(h) || !Math::nearZero(v)) {
        owner()->transform()->translate(Vector3(h, 0.f, v) * Time::deltaTime * mMoveSpeed);
    }
}

void PlayerMoveComponent::jumpMove() {
    if (mState != State::JUMP) {
        return;
    }
    auto t = owner()->transform();
    auto l = Vector3::lerp(t->getPosition(), mJumpTargetPosition, Time::deltaTime);
    t->setPosition(l);
}

void PlayerMoveComponent::jumpStop() {
    if (mState != State::JUMP) {
        return;
    }
    auto l = (owner()->transform()->getPosition() - mJumpTargetPosition).lengthSq();
    if (l < 0.01f) {
        mState = State::WALK;
    }
}
